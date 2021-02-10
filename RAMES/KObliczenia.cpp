#include "pch.h"

#include <fstream>
#include <algorithm>

#include "KObliczenia.h"
#include "mes.h"

using namespace std;

KObliczenia::KObliczenia()
{
	eWarunkiI = PAYNE_IRONS;
	mObliczeniaFlag = false;
	dane = new KDane();
	urMES = nullptr;
}

KObliczenia::~KObliczenia()
{
	if (urMES) delete urMES;
	if (dane) delete dane;
}

KMacierz* KObliczenia::StworzLokalnaMacierzSztywnosci(KElement1D* element)
{
	KMacierz* k = nullptr;

	switch (dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie())
	{
	case 2:
	{
		float kk[][2] = { {1,-1}, {-1,1} };
		k = new KMacierz(2, (float*)kk);
		(*k) *= element->Pobierzk() / element->Pobierzh();
		break; }
	case 3:
	{
		float k3[][3] = { {7,-8, 1}, {-8, 16, -8}, {1, -8, 7} };
		k = new KMacierz(3, (float*)k3);
		(*k) *= element->Pobierzk() / 3. / element->Pobierzh();
		break;
	}
	}
 // lambda/h * [1,-1; -1,1]

	return k;
}

KWektorK* KObliczenia::StworzLokalnyWektorNaprezen(KElement1D* element)
{
	KWektorK* p = nullptr;

	switch (dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie())
	{
	case 2:
	{
		float pp[2] = { 1,1 };
		p = new KWektorK(2, (float*)pp);
		break;
	}
	case 3:
	{
		float pp[3] = {1,4,1 };
		p = new KWektorK(3, (float*)pp);
		(*p) *= (double)element->Pobierzh() * element->Pobierzf() / 6.;
		break;
	}
	}
	(*p) *= element->Pobierzh() * element->Pobierzf() * 0.5 ; // 0.5*f*h* [1,1]

	return p;
}

/////////////////////////////////////////////////////
// Wstawia lokalna macierz sztywnosci elementu w odpowiednie
// miejsca globalnej macierzy sztywnosci

void KObliczenia::WypelnijK(KElement1D *e)
{
	KMacierz* k_e = StworzLokalnaMacierzSztywnosci(e);

	// okresl ile i ktore wezly w elemencie jest warunkami brzegowymi I rodzaju
	int il_wezlow_z_war_I = 0;
	vector<int> wezly_war_I; //wektor wezlow elementu bedacych warunkami I rodzaju

	for (int i(1); i <= e->PobierzWezly()->size(); ++i)
	{
		for (auto it_warI : *dane->PobierzWarunkiI())
		{
			if (e->PobierzWezel(i)->PobierzX() == it_warI.x) // jesli i-ty wezel jest warunkiem brzegowym
				wezly_war_I.push_back(i);
		}
	}

	// uzupelnij gloablna macierz sztywnosci macierza lokalna z wylaczeniem elementow o indeksach odpowiadajacych wezlom
	// bedacymi warunkami I rodzaju
	for (unsigned i = 1; i <= k_e->DajM(); i++)
	{
		if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == i; } ))
			continue;
		for (unsigned j = 1; j <= k_e->DajN(); j++)
		{
			if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == j; }))
				continue;
			(*urMES->PobierzA())(e->PobierzWezel(i)->PobierzNumer(), e->PobierzWezel(j)->PobierzNumer()) += k_e->DajIJ(i, j);
		}
	}

	delete k_e;
}

void KObliczenia::WypelnijP(KElement1D* e)
{
	KWektorK* p_e = StworzLokalnyWektorNaprezen(e);

	// okresl ile i ktore wezly w elemencie jest warunkami brzegowymi I rodzaju
	int il_wezlow_z_war_I = 0;
	vector<int> wezly_war_I; //wektor wezlow elementu bedacych warunkami I rodzaju

	for (int i(1); i <= e->PobierzWezly()->size(); ++i)
	{
		for (auto it_warI : *dane->PobierzWarunkiI())
		{
			if (e->PobierzWezel(i)->PobierzX() == it_warI.x) // jesli i-ty wezel jest warunkiem brzegowym
				wezly_war_I.push_back(i);
		}
	}

	for (short i = 1; i <= dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie(); i++)
	{
		if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == i; }))
			continue;
		(*urMES->PobierzB())(e->PobierzWezel(i)->PobierzNumer()) += p_e->DajI(i);
	}

	delete p_e;
}

void KObliczenia::WypelnijUkladRownan(KElement1D* e)
{
	switch (eWarunkiI)
	{
	case REDUKCJA:
		WypelnijUkladRownanAnalitycznie(e);
		break;
	case PAYNE_IRONS:
		WypelnijUkladRownanPayneIrons(e);
		break;
	default:
		break;
	}
}

void KObliczenia::WypelnijKWarunkamiBrzegowymi()
{
	//WypelnijWarunkamiIRodzaju();
	//WypelnijWarunkamiIIRodzaju();
}

void KObliczenia::WypelnijUkladRownanAnalitycznie(KElement1D* e)
{
	KMacierz* k_e = StworzLokalnaMacierzSztywnosci(e);
	KWektorK* p_e = StworzLokalnyWektorNaprezen(e);

	unsigned globalny_nr;
	float wyraz_warI;

	// okresl ktore wezly w elemencie sa warunkami brzegowymi I rodzaju; sluzy do redukcji ilosci rownan
	vector<unsigned> wezly_war_I; //wektor numerow wezlow elementu bedacych warunkami I rodzaju
	vector<int> il_wezlow_war_I_przed;
	for (int i(1); i <= e->PobierzWezly()->size(); ++i) // iteruj po wezlach elementu e
	{
		il_wezlow_war_I_przed.push_back(0);
		for (auto it_warI : *dane->PobierzWarunkiI()) // iteruj po globalnych warunkach I
		{
			if (e->PobierzWezel(i) == it_warI.pw) // jesli i-ty wezel elementu jest warunkiem brzegowym
				wezly_war_I.push_back(i);

			// okresl ile wezlow o mniejszych numerach w pozostalych elementach sa warunkami I rodzaju; sluzy do zmiany indeksow globalnego zredukowanego ukladu rownan
			if (it_warI.pw->PobierzNumer() < e->PobierzWezel(i)->PobierzNumer())
				++il_wezlow_war_I_przed[i - 1];
		}
	}

	// uzupelnij gloablna macierz sztywnosci macierza lokalna z wylaczeniem elementow o indeksach odpowiadajacych wezlom
	// bedacymi warunkami I rodzaju
	for (unsigned i = 1; i <= k_e->DajM(); i++) // iteruj po wierszach lokalnej macierzy sztywnosci k
	{
		if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == i; }))
			continue;
		for (unsigned j = 1; j <= k_e->DajN(); j++) // iteruj po kolumnach lokalnej macierzy sztywnosci k
		{
			if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == j; }))
				continue;
			(*urMES->PobierzA())(e->PobierzWezel(i)->PobierzNumer() - il_wezlow_war_I_przed[i - 1], e->PobierzWezel(j)->PobierzNumer() - il_wezlow_war_I_przed[j - 1]) += k_e->DajIJ(i, j);
		}
	}

	// uzupelnij gloablny wektor naprezen wektorem lokalnym z wylaczeniem elementow o indeksach odpowiadajacych wezlom
	// bedacymi warunkami I rodzaju
	for (unsigned i = 1; i <= p_e->DajM(); i++)
	{
		if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == i; }))
			continue;

		wyraz_warI = 0;
		globalny_nr = e->PobierzWezel(i)->PobierzNumer() - il_wezlow_war_I_przed[i - 1];
		for (auto it_wI : wezly_war_I) // jesli element posiada wezly bedace warunkami I rodzaju
		{
			wyraz_warI = (*k_e)(i, it_wI) * e->PobierzWezel(it_wI)->PobierzTemperature();
		}
		(*urMES->PobierzB())(globalny_nr) += p_e->DajI(i) - wyraz_warI;
	}

	delete k_e;
	delete p_e;
}

void KObliczenia::WypelnijUkladRownanPayneIrons(KElement1D* e)
{
	KMacierz* k_e = StworzLokalnaMacierzSztywnosci(e);
	KWektorK* p_e = StworzLokalnyWektorNaprezen(e);

	unsigned globalny_nr;
	const float stalaMnoznika = 1.0e8;
	float mnoznik;

	// okresl ktore wezly w elemencie sa warunkami brzegowymi I rodzaju
	vector<unsigned> wezly_war_I; //wektor numerow wezlow elementu bedacych warunkami I rodzaju
	for (int i(1); i <= e->PobierzWezly()->size(); ++i) // iteruj po wezlach elementu e
	{
		for (auto it_warI : *dane->PobierzWarunkiI()) // iteruj po globalnych warunkach I
		{
			if (e->PobierzWezel(i) == it_warI.pw) // jesli i-ty wezel elementu jest warunkiem brzegowym
				wezly_war_I.push_back(i);
		}
	}

	// uzupelnij gloablna macierz sztywnosci macierza lokalna z wylaczeniem elementow o indeksach odpowiadajacych wezlom
	// bedacymi warunkami I rodzaju
	for (unsigned i = 1; i <= k_e->DajM(); i++) // iteruj po wierszach lokalnej macierzy sztywnosci k
	{
		for (unsigned j = 1; j <= k_e->DajN(); j++) // iteruj po kolumnach lokalnej macierzy sztywnosci k
		{
			mnoznik = 1.0;
			for (auto it_wwI : wezly_war_I)
			{
				if (it_wwI == i && it_wwI == j)
					mnoznik = stalaMnoznika;
			}
			(*urMES->PobierzA())(e->PobierzWezel(i)->PobierzNumer(), e->PobierzWezel(j)->PobierzNumer()) += k_e->DajIJ(i, j) * mnoznik;
		}
	}

	// uzupelnij gloablny wektor naprezen wektorem lokalnym z wylaczeniem elementow o indeksach odpowiadajacych wezlom
	// bedacymi warunkami I rodzaju
	for (unsigned i = 1; i <= p_e->DajM(); i++)
	{
		globalny_nr = e->PobierzWezel(i)->PobierzNumer();
		for (auto it_wwI : wezly_war_I)
		{
			if (it_wwI == i)
			{
				(*p_e)(i) = (*k_e)(i, i) * stalaMnoznika * e->PobierzWezel(i)->PobierzTemperature();
			}
		}
		(*urMES->PobierzB())(globalny_nr) += p_e->DajI(i);
	}

	delete k_e;
	delete p_e;
}

KWektorK* KObliczenia::Licz(KDane* idane)
{
	if (dane)
		delete dane;
	dane = idane;
	dane->PobierzSiatke()->Generuj();

	unsigned il_warunkow_I = (unsigned)dane->PobierzWarunkiI()->size();;
	unsigned rozmiar = (unsigned)dane->PobierzSiatke()->vpWezly.size();

	// tworzenie i wypelnianie ukladu rownan
	if (eWarunkiI == REDUKCJA)
		rozmiar -= il_warunkow_I;

	KMacierz K(rozmiar); // globalna macierz sztywnosci
	KWektorK P(rozmiar); // globalny wektor naprezen
	KWektorK X(rozmiar);

	urMES = new KUkladRownan(&K, &X, &P);

	// iteruj po wszystkich elementach i uzupelnij globalna macierz sztywnosci macierzami lokalnymi
	for (auto it_e =  dane->PobierzSiatke()->PobierzElementy()->begin(); it_e != dane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
	{

		//KMacierz k(wymiar);
		//for (unsigned i = 1; i <= wymiar; i++)
		//{
		//	for (unsigned j = 1; j <= wymiar; j++)
		//	{
		//		double* f_ksi = new double[wymiar];
		//		for (int ii = 0; ii < n; ii++)
		//		{
		//			f_ksi[ii] = k_e * psiprim[i - 1](mes::ksi[n][ii]) * psiprim[j - 1](mes::ksi[n][ii] + b_e * psi[i - 1](mes::ksi[n][ii] * psi[j - 1](mes::ksi[n][ii])));
		//		}
		//		k(i, j) = 0.5 * h_e * mes::calka_11(f_ksi, n); // 
		//		delete f_ksi;
		//	}
		//}
		WypelnijUkladRownan(&*it_e);
	}

	// uwzglednij warunki brzegowe i zrodla punktowe w wektorze naprezen
	WypelnijZrodlamiPunktowymi(urMES);
	WypelnijWarunkamiIIRodzaju(urMES);
	WypelnijWarunkamiKonwekcyjnymi(urMES);

	// rozwiaz uklad rownan
	urMES->Rozwiaz();
	UstawTemperatureWezlow();
	mObliczeniaFlag = true;

	return urMES->PobierzX();
}

void KObliczenia::WypiszWynik(ostream& iplik, bool tylkoWezly)
{
	tylkoWezly = false;
	iplik << "x\tT\n";

	if (tylkoWezly)
	{
		for (auto it_w : *dane->PobierzSiatke()->PobierzWezly())
		{
			iplik << setprecision(6) << it_w->PobierzX() << "\t" << it_w->PobierzTemperature() << "\n";
		}
	}
	else
	{
		int gestosc = 6;
		float X, Xi, Xj, Xk, Ti, Tj, Tk, dx, TT;

		if (dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() == 2)
		{
			for (auto it_e : *dane->PobierzSiatke()->PobierzElementy())
			{
				Xi = it_e.PobierzWezel(1)->PobierzX();
				Xj = it_e.PobierzWezel(2)->PobierzX();
				Ti = it_e.PobierzWezel(1)->PobierzTemperature();
				Tj = it_e.PobierzWezel(2)->PobierzTemperature();

				dx = it_e.Pobierzh() / gestosc;

				for (int i(0); i < gestosc; ++i)
				{
					X = Xi + i * dx;
					TT = T2(X, Xi, Ti, Xj, Tj);
					iplik << setprecision(6) << X << "\t" << TT << "\n";
				}
			}
			iplik << setprecision(6) << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(2)->PobierzX() << "\t" << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(2)->PobierzTemperature() << "\n";
		}
		else
		{
			for (auto it_e : *dane->PobierzSiatke()->PobierzElementy())
			{
				Xi = it_e.PobierzWezel(1)->PobierzX();
				Xj = it_e.PobierzWezel(2)->PobierzX();
				Xk = it_e.PobierzWezel(3)->PobierzX();
				Ti = it_e.PobierzWezel(1)->PobierzTemperature();
				Tj = it_e.PobierzWezel(2)->PobierzTemperature();
				Tk = it_e.PobierzWezel(3)->PobierzTemperature();

				dx = it_e.Pobierzh() / gestosc;

				for (int i(0); i < gestosc; ++i)
				{
					X = Xi + i * dx;
					TT = T3(X, Xi, Ti, Xj, Tj, Xk, Tk);
					iplik << setprecision(6) << X << "\t" << TT << "\n";
				}
			}
			iplik << setprecision(6) << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(3)->PobierzX() << "\t" << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(3)->PobierzTemperature() << "\n";
		}

	}
}

void KObliczenia::UstawUwzglednianieWarunkowI(int uwzglednianieWarunkowI)
{
	eWarunkiI = EwarunkiI(uwzglednianieWarunkowI);
}


void KObliczenia::WypelnijWarunkamiIRodzaju()
{
}

void KObliczenia::WypelnijWarunkamiIIRodzaju(KUkladRownan* ur)
{
	unsigned il_wezlow_z_war_I = 0;
	float q;

	for (auto it_wII : *dane->PobierzWarunkiII())
	{
		if (eWarunkiI == REDUKCJA)
		{
			il_wezlow_z_war_I = 0;
			// sprawdz ile wezlow z warunkami I rodzaju  znajduje sie przed wezlem z warunkiem konwekcyjnym
			for (auto it_wI : *dane->PobierzWarunkiI())
			{
				if (it_wI.pw->PobierzNumer() < it_wII.pWii->PobierzNumer()) // jesli i-ty wezel jest warunkiem brzegowym
					il_wezlow_z_war_I++;
			}
		}
		// wypelnij globalny wektor naprezen warunkiem
		q = it_wII.q;
		(*ur->PobierzB())(it_wII.pWii->PobierzNumer() - il_wezlow_z_war_I) += q;
	}
}

void KObliczenia::WypelnijWarunkamiKonwekcyjnymi(KUkladRownan* ur)
{
	unsigned il_wezlow_z_war_I = 0;
	float h, T_inf;

	for (auto it_wk : *dane->PobierzWarunkiKonwekcyjne())
	{
		if (eWarunkiI == REDUKCJA)
		{
			il_wezlow_z_war_I = 0;
			// sprawdz ile wezlow z warunkami I rodzaju  znajduje sie przed wezlem z warunkiem konwekcyjnym
			for (auto it_wI : *dane->PobierzWarunkiI())
			{
				if (it_wI.pw->PobierzNumer() < it_wk.wk->PobierzNumer()) // jesli i-ty wezel jest warunkiem brzegowym
					il_wezlow_z_war_I++;
			}
		}

		// wypelnij globalny wektor naprezen warunkiem
		h = it_wk.h;
		T_inf = it_wk.T_inf;
		(*ur->PobierzB())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += h * T_inf;
		(*ur->PobierzA())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I, it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += it_wk.h ;
	}
}

void KObliczenia::WypelnijZrodlamiPunktowymi(KUkladRownan* ur)
{
	unsigned il_wezlow_z_war_I;

	for (auto it_zp : *dane->PobierzSiatke()->PobierzZrodlaPunktowe())
	{
		il_wezlow_z_war_I = 0;
		if (eWarunkiI == REDUKCJA)
		{
			for (auto it_wI : *dane->PobierzWarunkiI())
			{
				for (auto it_warI : *dane->PobierzWarunkiI())
				{
					if (it_wI.x == it_warI.x) // jesli i-ty wezel jest warunkiem brzegowym
						il_wezlow_z_war_I++;
				}
			}
		}
		(*ur->PobierzB())(it_zp.pz->PobierzNumer() - il_wezlow_z_war_I) += it_zp.g;
	}
}

void KObliczenia::WypelnijZrodlamiRozciaglymi()
{
}

void KObliczenia::UstawTemperatureWezlow()
{
	unsigned wezlyI = 0;
	unsigned i = 1;
	for (auto it_w : *dane->PobierzSiatke()->PobierzWezly())
	{
		if (it_w->PobierzTemperature() > 0)
		{
			if (eWarunkiI != REDUKCJA)
				++i;
			continue;
		}
		it_w->UstawTemperature((*urMES->PobierzX())(i));
		++i;
	}
}

KDane* KObliczenia::PobierzDane()
{
	return dane;
}

KUkladRownan* KObliczenia::PobierzUkladRownan()
{
	return urMES;
}

KMacierz* KObliczenia::PobierzGlobalnaMacierzSztywnosci()
{
	return urMES->PobierzA();
}


KWektorK* KObliczenia::PobierzGlobalnyWektorNaprezen()
{
	return urMES->PobierzB();
}
