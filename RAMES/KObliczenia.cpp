#include "pch.h"

#include <fstream>
#include <algorithm>

#include "KObliczenia.h"
#include "KMacierzSPF.h"

#include "mes.h"

using namespace std;

KObliczenia::KObliczenia()
{
	mfTmin = -1;
	mfTmax = -1;
	mWyniki = 0;
	mWynikiLiczbaPunktow = 100;
	mWynikiStalyKrok = 0.1;
	mWynikiZakresMax = 0;
	mWynikiZakresMin = 0;
	mGestoscAproksymacji = 1;
	eWarunkiI = EwarunkiI::PAYNE_IRONS;
	pDane = nullptr;
}

KObliczenia::~KObliczenia()
{
}

void KObliczenia::UstawWynikiRozklad(int wyniki)
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

	switch (pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie())
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

	switch (pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie())
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
		for (auto it_warI = pDane->PobierzWarunkiI()->begin(); it_warI != pDane->PobierzWarunkiI()->end(); ++it_warI)
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
			(*mUrMES.PobierzA())(e->PobierzWezel(i)->PobierzNumer(), e->PobierzWezel(j)->PobierzNumer()) += k_e->DajIJ(i, j);
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
		for (auto it_warI = pDane->PobierzWarunkiI()->begin(); it_warI != pDane->PobierzWarunkiI()->end(); ++it_warI)
		{
			if (e->PobierzWezel(i)->PobierzX() == it_warI->x) // jesli i-ty wezel jest warunkiem brzegowym
				wezly_war_I.push_back(i);
		}
	}

	for (short i = 1; i <= pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie(); i++)
	{
		if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == i; }))
			continue;
		(*mUrMES.PobierzB())(e->PobierzWezel(i)->PobierzNumer()) += p_e->DajI(i);
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
		for (auto it_warI : *pDane->PobierzWarunkiI()) // iteruj po globalnych warunkach I
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
		for (unsigned j = i; j <= k_e->DajN(); j++) // iteruj po kolumnach lokalnej macierzy sztywnosci k
		{
			if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == j; }))
				continue;
			(*mUrMES.PobierzA())(e->PobierzWezel(i)->PobierzNumer() - il_wezlow_war_I_przed[i - 1], e->PobierzWezel(j)->PobierzNumer() - il_wezlow_war_I_przed[j - 1]) += (*k_e)(i, j);
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
		(*mUrMES.PobierzB())(globalny_nr) += (*p_e)(i) - wyraz_warI;
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
		for (auto it_warI = pDane->PobierzWarunkiI()->begin(); it_warI != pDane->PobierzWarunkiI()->end(); ++it_warI) // iteruj po globalnych warunkach I
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
			(*mUrMES.PobierzA())(e->PobierzWezel(i)->PobierzNumer(), e->PobierzWezel(j)->PobierzNumer()) += (*k_e)(i, j) * mnoznik;
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
		(*mUrMES.PobierzB())(globalny_nr) += p_e->DajI(i);
	}
	delete k_e;
	delete p_e;
}

int KObliczenia::Licz(KDane* idane)
{
	pDane = idane;
	//pDane->PobierzSiatke()->Generuj();

	unsigned il_warunkow_I = (unsigned)pDane->PobierzWarunkiI()->size();
	unsigned rozmiar = (unsigned)pDane->PobierzSiatke()->vpWezly.size();

	// tworzenie i wypelnianie ukladu rownan
	if (eWarunkiI == EwarunkiI::REDUKCJA)
		rozmiar -= il_warunkow_I;

	KMacierzSPF K(rozmiar, pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() * 2 - 1); // globalna macierz sztywrz(nosci
	KWektorK P(rozmiar); // globalny wektor naprezen
	KWektorK X(rozmiar);

	mUrMES.Ustaw(&K, &X, &P);

	// iteruj po wszystkich elementach i uzupelnij globalna macierz sztywnosci macierzami lokalnymi
	for (auto it_e =  pDane->PobierzSiatke()->PobierzElementy()->begin(); it_e != pDane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
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
	mUrMES.Rozwiaz();
	UstawTemperatureWezlow();
	//pDane->OdstosujSkale();

	return 0;
}

void KObliczenia::WypiszWynik(ostream& iplik) const
{
	using namespace mes::galerkin::funkcje_ksztaltu;

	UstawFormatWynikow(iplik);
	iplik << "x [m]" << "\t\t" << "T [K]" << "\n";
	
	double X, dx, TT, Xc;

	switch (mWyniki)
	{
	case 0: //temperatura tylko w wezlach
	{
			for (auto it_w : *pDane->PobierzSiatke()->PobierzWezly())
			{
				if (it_w->PobierzX() >= mWynikiZakresMin && it_w->PobierzX() <= mWynikiZakresMax)
					iplik << it_w->PobierzX() << "\t" << it_w->PobierzTemperature() << "\n";
			}
			break;
	}
	case 1: // apkrosmyacja w punktach niewwezlowych
	{
		if (pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() == 2)
		{
			for (auto it_e = pDane->PobierzSiatke()->PobierzElementy()->begin(); it_e != pDane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
			{
				dx = it_e->Pobierzh() / (mGestoscAproksymacji + 2);
				X = it_e->PobierzWezel(1)->PobierzX();
				for (int i(0); i < mGestoscAproksymacji; ++i)
				{
					if (X >= mWynikiZakresMin && X <= mWynikiZakresMax)
					{
						TT = T2(X, it_e->PobierzWezel(1)->PobierzX(), it_e->PobierzWezel(1)->PobierzTemperature(), it_e->PobierzWezel(2)->PobierzX(),
							 it_e->PobierzWezel(2)->PobierzTemperature());
						iplik << X << "\t" << TT << "\n";
					}
					X += dx;
				}
			}
		}
		else if (pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() == 3)
		{
			for (auto it_e = pDane->PobierzSiatke()->PobierzElementy()->begin(); it_e != pDane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
			{
				dx = it_e->Pobierzh() / (mGestoscAproksymacji + 3);
				X = it_e->PobierzWezel(1)->PobierzX();
				for (int i(0); i < mGestoscAproksymacji; ++i)
				{
					if (X >= mWynikiZakresMin && X <= mWynikiZakresMax)
					{
						TT = T3(X, it_e->PobierzWezel(1)->PobierzX(), it_e->PobierzWezel(1)->PobierzTemperature(), it_e->PobierzWezel(2)->PobierzX(), it_e->PobierzWezel(2)->PobierzTemperature(), it_e->PobierzWezel(3)->PobierzX(), it_e->PobierzWezel(3)->PobierzTemperature());
						iplik << X << "\t" << TT << "\n";
					}
					X += dx;
				}
			}
		}
		break;
	}
	case 2: // liczba punktow
	{
		dx = (mWynikiZakresMax - mWynikiZakresMin) / (mWynikiLiczbaPunktow - 1);
		break;
	}
	case 3: // staly krok
	{
		dx = mWynikiStalyKrok;
		break;
	}
	default:
	{
		break;
	}

	}

	if (mWyniki == 2 || mWyniki == 3) 
	{
		X = mWynikiZakresMin;

		if (pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() == 2)
		{
			for (auto it_e = pDane->PobierzSiatke()->PobierzElementy()->begin(); it_e != pDane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
			{
				Xc = (*(it_e->PobierzWezly()->end() - 1))->PobierzX();
				while (X <= Xc && X >= mWynikiZakresMin && X <= mWynikiZakresMax) {
					TT = T2(X, it_e->PobierzWezel(1)->PobierzX(), it_e->PobierzWezel(1)->PobierzTemperature(), it_e->PobierzWezel(2)->PobierzX(),
						it_e->PobierzWezel(2)->PobierzTemperature());
					iplik << X << "\t" << TT << "\n";
					X += dx;
				}
			}
		}
		else if (pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() == 3)
		{
			for (auto it_e = pDane->PobierzSiatke()->PobierzElementy()->begin(); it_e != pDane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
			{
				Xc = (*(it_e->PobierzWezly()->end() - 1))->PobierzX();
				while (X <= Xc && X >= mWynikiZakresMin && X <= mWynikiZakresMax) {
					TT = T3(X, it_e->PobierzWezel(1)->PobierzX(), it_e->PobierzWezel(1)->PobierzTemperature(), it_e->PobierzWezel(2)->PobierzX(), it_e->PobierzWezel(2)->PobierzTemperature(), it_e->PobierzWezel(3)->PobierzX(), it_e->PobierzWezel(3)->PobierzTemperature());
					iplik << X << "\t" << TT << "\n";
					X += dx;
				}
			}
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

	for (auto it_wII : *pDane->PobierzWarunkiII())
	{
		if (eWarunkiI == EwarunkiI::REDUKCJA)
		{
			il_wezlow_z_war_I = 0;
			// sprawdz ile wezlow z warunkami I rodzaju  znajduje sie przed wezlem z warunkiem konwekcyjnym
			for (auto it_wI : *pDane->PobierzWarunkiI())
			{
				if (it_wI.pw->PobierzNumer() < it_wII.pWii->PobierzNumer()) // jesli i-ty wezel jest warunkiem brzegowym
					il_wezlow_z_war_I++;
			}
		}
		// wypelnij globalny wektor naprezen warunkiem
		q = it_wII.q;
		(*mUrMES.PobierzB())(it_wII.pWii->PobierzNumer() - il_wezlow_z_war_I) += q;
	}
}

void KObliczenia::WypelnijWarunkamiKonwekcyjnymi()
{
	unsigned il_wezlow_z_war_I = 0;
	double h, T_inf;

	for (auto it_wk : *pDane->PobierzWarunkiKonwekcyjne())
	{
		if (eWarunkiI == EwarunkiI::REDUKCJA)
		{
			il_wezlow_z_war_I = 0;
			// sprawdz ile wezlow z warunkami I rodzaju  znajduje sie przed wezlem z warunkiem konwekcyjnym
			for (auto it_wI : *pDane->PobierzWarunkiI())
			{
				if (it_wI.pw->PobierzNumer() < it_wk.wk->PobierzNumer()) // jesli i-ty wezel jest warunkiem brzegowym
					il_wezlow_z_war_I++;
			}
		}

		// wypelnij globalny wektor naprezen warunkiem
		h = it_wk.h;
		T_inf = it_wk.T_inf;
		(*mUrMES.PobierzB())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += h * T_inf;
		(*mUrMES.PobierzA())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I, it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += it_wk.h ;
	}
}

void KObliczenia::WypelnijZrodlamiPunktowymi()
{
	unsigned il_wezlow_z_war_I;

	for (auto it_zp : *pDane->PobierzSiatke()->PobierzZrodlaPunktowe())
	{
		il_wezlow_z_war_I = 0;
		if (eWarunkiI == EwarunkiI::REDUKCJA)
		{
			for (auto it_wI = pDane->PobierzWarunkiI()->begin(); it_wI != pDane->PobierzWarunkiI()->end(); ++it_wI)
			{
				for (auto it_warI = pDane->PobierzWarunkiI()->begin(); it_warI != pDane->PobierzWarunkiI()->end(); ++it_warI)
				{
					if (it_wI->x == it_warI->x) // jesli i-ty wezel jest warunkiem brzegowym
						il_wezlow_z_war_I++;
				}
			}
		}
		(*mUrMES.PobierzB())(it_zp.pz->PobierzNumer() - il_wezlow_z_war_I) += it_zp.g;
	}
}

void KObliczenia::WypelnijZrodlamiRozciaglymi()
{
}

void KObliczenia::UstawTemperatureWezlow()
{
	double t;
	unsigned wezlyI = 0;
	unsigned i = 1;
	mfTmin = (*mUrMES.PobierzX())(1);
	mfTmax = mfTmin;

	for (auto it_w = pDane->PobierzSiatke()->PobierzWezly()->begin(); it_w < pDane->PobierzSiatke()->PobierzWezly()->end(); ++it_w)
	{
		if ((*it_w)->PobierzTemperature() > 0)
		{
			if (eWarunkiI != EwarunkiI::REDUKCJA)
				++i;
			continue;
		}
		t = (*mUrMES.PobierzX())(i);

		if (t < mfTmin) mfTmin = t;
		else if (t > mfTmax) mfTmax = t;
		(*it_w)->UstawTemperature(t);
		++i;
	}
	for (auto itpw = pDane->PobierzWarunkiI()->begin(); itpw != pDane->PobierzWarunkiI()->end(); ++itpw)
	{
		t = itpw->pw->PobierzTemperature();
		if (t < mfTmin)
			mfTmin = t;
		else if (t > mfTmax)
			mfTmax = t;
	}
}

void KObliczenia::UstawFormatWynikow(std::ostream& stream) const
{
	stream << fixed << setprecision(6);
}

KDane* KObliczenia::PobierzDane()
{
	return pDane;
}

KUkladRownan* KObliczenia::PobierzUkladRownan()
{
	return &mUrMES;
}

KMacierz* KObliczenia::PobierzGlobalnaMacierzSztywnosci()
{
	return mUrMES.PobierzA();
}


KWektorK* KObliczenia::PobierzGlobalnyWektorNaprezen()
{
	return mUrMES.PobierzB();
}

void KObliczenia::WyznaczTwZakresie(double xp, double xk, unsigned n, double* t) const
{
	using namespace mes::galerkin::funkcje_ksztaltu;

	if (xp < pDane->PobierzSiatke()->PobierzXmin() || xk > pDane->PobierzSiatke()->PobierzXmax())
		throw runtime_error("wspolrzedna poza zakresem struktury");

	double X, Xi, Xj, Xk, Ti, Tj, Tk, Xc, dx;
	unsigned i = 0;
	X = xp;
	dx = (xk - xp) / (n - 1);
	if (pDane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie() == 2)
	{
		for (auto it_e = pDane->PobierzSiatke()->PobierzElementy()->begin(); it_e != pDane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
		{
			Xc = (*(it_e->PobierzWezly()->end() - 1))->PobierzX();
			Xi = it_e->PobierzWezel(1)->PobierzX();
			Xj = it_e->PobierzWezel(2)->PobierzX();
			Ti = it_e->PobierzWezel(1)->PobierzTemperature();
			Tj = it_e->PobierzWezel(2)->PobierzTemperature();
			while (X <= Xc)
			{
				t[i]  = T2(X, Xi, Ti, Xj, Tj);
				i++;
				X = xp + i * dx;
			}
		}
	}
	else
	{
		for (auto it_e = pDane->PobierzSiatke()->PobierzElementy()->begin(); it_e != pDane->PobierzSiatke()->PobierzElementy()->end(); ++it_e)
		{
			Xc = (*(it_e->PobierzWezly()->end() - 1))->PobierzX();
			Xi = it_e->PobierzWezel(1)->PobierzX();
			Xj = it_e->PobierzWezel(2)->PobierzX();
			Xk = it_e->PobierzWezel(3)->PobierzX();
			Ti = it_e->PobierzWezel(1)->PobierzTemperature();
			Tj = it_e->PobierzWezel(2)->PobierzTemperature();
			Tk = it_e->PobierzWezel(3)->PobierzTemperature();
			while (X <= Xc)
			{
				t[i] = T3(X, Xi, Ti, Xj, Tj, Xk, Tk);
				i++;
				X = xp+ i * dx;
			}
		}
	}
}
void KObliczenia::UstawWynikiCzyZakres(bool zakres) {
	mbWynikiCzyZakres = zakres;
}
void KObliczenia::UstawWynikiZakres(double zakresMin, double zakresMax)
{
	if (mbWynikiCzyZakres)
	{
		mWynikiZakresMin = zakresMin;
		mWynikiZakresMax = zakresMax;
	}
	else
	{
		mWynikiZakresMin = pDane->PobierzSiatke()->PobierzXmin();
		mWynikiZakresMax = pDane->PobierzSiatke()->PobierzXmax();
	}
}
void KObliczenia::UstawWynikiLiczbaPunktow(unsigned liczbaPunktow)
{
	mWynikiLiczbaPunktow = liczbaPunktow;
}
void KObliczenia::UstawWynikiStalyKrok(double krok)
{
	mWynikiStalyKrok = krok;
}
