#include "pch.h"

#include <fstream>
#include <algorithm>

#include "KObliczenia.h"
#include "KMacierzSPF.h"

#include "mes.h"

using namespace std;

KObliczenia::KObliczenia()
{
	mWyniki = 0;
	mGestoscAproksymacji = 1;
	eWarunkiI = EwarunkiI::PAYNE_IRONS;
	dane = new KDane();
	urMES = nullptr;
}

KObliczenia::~KObliczenia()
{
	if (urMES) delete urMES;
	if (dane) delete dane;
}

void KObliczenia::UstawWynikiTylkoWezly(int wyniki)
{
	mWyniki = wyniki;
}

void KObliczenia::UstawWynikiGestoscAproksymacji(int gestosc)
{
	mGestoscAproksymacji = gestosc;
}

KMacierz* KObliczenia::StworzLokalnaMacierzSztywnosci(KElement1D* element)
{
	KMacierz* k = nullptr;

	switch (dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie())
	{
	case 2:
	{
		double kk[][2] = { {1,-1}, {-1,1} };
		k = new KMacierz(2, (double*)kk);
		(*k) *= element->Pobierzk() / element->Pobierzh();
		break; }
	case 3:
	{
		double k3[][3] = { {7,-8, 1}, {-8, 16, -8}, {1, -8, 7} };
		k = new KMacierz(3, (double*)k3);
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
		double pp[2] = { 1,1 };
		p = new KWektorK(2, (double*)pp);
		(*p) *= (double)element->Pobierzh() * element->Pobierzf() * 0.5 ; // 0.5*f*h* [1,1]
		break;
	}
	case 3:
	{
		double pp[3] = {1,4,1 };
		p = new KWektorK(3, (double*)pp);
		(*p) *= (double)element->Pobierzh() * element->Pobierzf() / 6.;
		break;
	}
	}
	//(*p) *= element->Pobierzh() * element->Pobierzf() * 0.5 ; // 0.5*f*h* [1,1]

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
		for (auto it_warI = dane->PobierzWarunkiI()->begin(); it_warI != dane->PobierzWarunkiI()->end(); ++it_warI)
		{
			if (e->PobierzWezel(i)->PobierzX() == it_warI->x) // jesli i-ty wezel jest warunkiem brzegowym
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
		for (auto it_warI = dane->PobierzWarunkiI()->begin(); it_warI != dane->PobierzWarunkiI()->end(); ++it_warI)
		{
			if (e->PobierzWezel(i)->PobierzX() == it_warI->x) // jesli i-ty wezel jest warunkiem brzegowym
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

void KObliczenia::WstawMacierzLokalna(KElement1D* e)
{
	switch (eWarunkiI)
	{
	case EwarunkiI::REDUKCJA:
		WstawMacierzLokalnaRedukcja(e);
		break;
	case EwarunkiI::PAYNE_IRONS:
		WstawMacierzLokalnaPayneIrons(e);
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

void KObliczenia::WstawMacierzLokalnaRedukcja(KElement1D* e)
{
	KMacierz* k_e = StworzLokalnaMacierzSztywnosci(e);
	KWektorK* p_e = StworzLokalnyWektorNaprezen(e);

	unsigned globalny_nr;
	double wyraz_warI;

	// okresl ktore wezly w elemencie sa warunkami brzegowymi I rodzaju; sluzy do redukcji ilosci rownan
	vector<unsigned> wezly_war_I; //wektor numerow wezlow elementu bedacych warunkami I rodzaju
	vector<int> il_wezlow_war_I_przed;
	for (unsigned i(1); i <= e->PobierzWezly()->size(); ++i) // iteruj po wezlach elementu e
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

void KObliczenia::WstawMacierzLokalnaPayneIrons(KElement1D* e)
{
	KMacierz* k_e = StworzLokalnaMacierzSztywnosci(e);
	KWektorK* p_e = StworzLokalnyWektorNaprezen(e);

	unsigned globalny_nr;
	const double stalaMnoznika = 1.0e8;
	double mnoznik;

	// okresl ktore wezly w elemencie sa warunkami brzegowymi I rodzaju
	vector<unsigned> wezly_war_I; //wektor numerow wezlow elementu bedacych warunkami I rodzaju
	for (int i(1); i <= e->PobierzWezly()->size(); ++i) // iteruj po wezlach elementu e
	{
		for (auto it_warI = dane->PobierzWarunkiI()->begin(); it_warI != dane->PobierzWarunkiI()->end(); ++it_warI) // iteruj po globalnych warunkach I
		{
			if (e->PobierzWezel(i) == it_warI->pw) // jesli i-ty wezel elementu jest warunkiem brzegowym
				wezly_war_I.push_back(i);
		}
	}

	// uzupelnij gloablna macierz sztywnosci macierza lokalna z wylaczeniem elementow o indeksach odpowiadajacych wezlom
	// bedacymi warunkami I rodzaju
	for (unsigned i = 1; i <= k_e->DajM(); i++) // iteruj po wierszach lokalnej macierzy sztywnosci k
	{
		for (unsigned j = i; j <= k_e->DajN(); j++) // iteruj po kolumnach lokalnej macierzy sztywnosci k
		{
			mnoznik = 1.0;
			for (auto it_wwI : wezly_war_I)
			{
				if (it_wwI == i && it_wwI == j)
					mnoznik = stalaMnoznika;
			}
			(*urMES->PobierzA())(e->PobierzWezel(i)->PobierzNumer(), e->PobierzWezel(j)->PobierzNumer()) += (*k_e)(i, j) * mnoznik;
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

int KObliczenia::Licz(KDane* idane)
{
	dane = idane;
	dane->PobierzSiatke()->Generuj();

	unsigned il_warunkow_I = (unsigned)dane->PobierzWarunkiI()->size();;
	unsigned rozmiar = (unsigned)dane->PobierzSiatke()->vpWezly.size();

	// tworzenie i wypelnianie ukladu rownan
	if (eWarunkiI == EwarunkiI::REDUKCJA)
		rozmiar -= il_warunkow_I;

	KMacierzSPF K(rozmiar, dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() * 2 - 1); // globalna macierz sztywrz(nosci
	KWektorK P(rozmiar); // globalny wektor naprezen
	KWektorK X(rozmiar);

	if (urMES) delete urMES;
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
		WstawMacierzLokalna(&*it_e);
	}


	// uwzglednij warunki brzegowe i zrodla punktowe w wektorze naprezen
	WypelnijZrodlamiPunktowymi();
	WypelnijWarunkamiIIRodzaju();
	WypelnijWarunkamiKonwekcyjnymi();

	// rozwiaz uklad rownan
	urMES->Rozwiaz();
	UstawTemperatureWezlow();
	//dane->OdstosujSkale();

	return 0;
}

void KObliczenia::WypiszWynik(ostream& iplik) const
{
	using namespace mes::galerkin::funkcje_ksztaltu;
	UstawFormatWynikow(iplik);
	iplik << "x [m]" << "\t\t" << "T [K]" << "\n";

	if (mWyniki == 0)
	{
		for (auto it_w : *dane->PobierzSiatke()->PobierzWezly())
		{
			iplik << it_w->PobierzX() << "\t" << it_w->PobierzTemperature() << "\n";
		}
	}
	else if (mWyniki == 1)
	{
		double X, Xi, Xj, Xk, Ti, Tj, Tk, dx, TT;

		if (dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() == 2)
		{
			for (auto it_e : *dane->PobierzSiatke()->PobierzElementy())
			{
				Xi = it_e.PobierzWezel(1)->PobierzX();
				Xj = it_e.PobierzWezel(2)->PobierzX();
				Ti = it_e.PobierzWezel(1)->PobierzTemperature();
				Tj = it_e.PobierzWezel(2)->PobierzTemperature();

				dx = it_e.Pobierzh() / mGestoscAproksymacji;

				for (int i(0); i < mGestoscAproksymacji; ++i)
				{
					X = Xi + i * dx;
					TT = T2(X, Xi, Ti, Xj, Tj);
					iplik << X  << TT << "\n";
				}
			}
			iplik << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(2)->PobierzX() << "\t" << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(2)->PobierzTemperature() << "\n";
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

				dx = it_e.Pobierzh() / mGestoscAproksymacji;

				for (int i(0); i < mGestoscAproksymacji; ++i)
				{
					X = Xi + i * dx;
					TT = T3(X, Xi, Ti, Xj, Tj, Xk, Tk);
					iplik  << X << "\t" << TT << "\n";
				}
			}
			iplik << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(3)->PobierzX() << "\t" << (dane->PobierzSiatke()->PobierzElementy()->end() - 1)->PobierzWezel(3)->PobierzTemperature() << "\n";
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

void KObliczenia::WypelnijWarunkamiIIRodzaju()
{
	unsigned il_wezlow_z_war_I = 0;
	double q;

	for (auto it_wII : *dane->PobierzWarunkiII())
	{
		if (eWarunkiI == EwarunkiI::REDUKCJA)
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
		(*urMES->PobierzB())(it_wII.pWii->PobierzNumer() - il_wezlow_z_war_I) += q;
	}
}

void KObliczenia::WypelnijWarunkamiKonwekcyjnymi()
{
	unsigned il_wezlow_z_war_I = 0;
	double h, T_inf;

	for (auto it_wk : *dane->PobierzWarunkiKonwekcyjne())
	{
		if (eWarunkiI == EwarunkiI::REDUKCJA)
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
		(*urMES->PobierzB())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += h * T_inf;
		(*urMES->PobierzA())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I, it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += it_wk.h ;
	}
}

void KObliczenia::WypelnijZrodlamiPunktowymi()
{
	unsigned il_wezlow_z_war_I;

	for (auto it_zp : *dane->PobierzSiatke()->PobierzZrodlaPunktowe())
	{
		il_wezlow_z_war_I = 0;
		if (eWarunkiI == EwarunkiI::REDUKCJA)
		{
			for (auto it_wI = dane->PobierzWarunkiI()->begin(); it_wI != dane->PobierzWarunkiI()->end(); ++it_wI)
			{
				for (auto it_warI = dane->PobierzWarunkiI()->begin(); it_warI != dane->PobierzWarunkiI()->end(); ++it_warI)
				{
					if (it_wI->x == it_warI->x) // jesli i-ty wezel jest warunkiem brzegowym
						il_wezlow_z_war_I++;
				}
			}
		}
		(*urMES->PobierzB())(it_zp.pz->PobierzNumer() - il_wezlow_z_war_I) += it_zp.g;
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
			if (eWarunkiI != EwarunkiI::REDUKCJA)
				++i;
			continue;
		}
		it_w->UstawTemperature((*urMES->PobierzX())(i));
		++i;
	}
}

void KObliczenia::UstawFormatWynikow(std::ostream& stream) const
{
	stream << fixed << setprecision(6);
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
