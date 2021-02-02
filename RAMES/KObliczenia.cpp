#include "pch.h"

#include <fstream>
#include <algorithm>

#include "KObliczenia.h"
#include "mes.h"

using namespace std;

KObliczenia::KObliczenia()
{
	dane = new KDane();
	urMES = new KUkladRownan();
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
		float kk[][2] = { {1,-1}, {-1,1} };
		k = new KMacierz(2,(float*) kk);
		break;
	}
	(*k) *= element->Pobierzk() / element->Pobierzh(); // lambda/h * [1,-1; -1,1]

	ofstream plik("ke.txt");
	k->Wypisz(plik);

	return k;
}

KWektor* KObliczenia::StworzLokalnyWektorNaprezen(KElement1D* element)
{
	KWektor* p = nullptr;

	switch (dane->PobierzSiatke()->PobierzLiczbeWezlowWelemencie())
	{
	case 2:
		float pp[2] = { 1,1 };
		p = new KWektor(2, (float*)pp);
		break;
	}
	(*p) *= 0.5 * element->Pobierzh() * element->Pobierzf(); // 0.5*f*h* [1,1]

	ofstream plik("pe.txt");
	p->Wypisz(plik);

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

	//k_e->Wypisz(plik);

	delete k_e;
}

void KObliczenia::WypelnijP(KElement1D* e)
{
	KWektor* p_e = StworzLokalnyWektorNaprezen(e);

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
		(*urMES->PobierzY())(e->PobierzWezel(i)->PobierzNumer()) += p_e->DajIJ(i);
	}
	ofstream plik("P.txt");
	p_e->Wypisz(plik);
	delete p_e;
}

void KObliczenia::WypelnijUkladRownan(KElement1D* e)
{
	KMacierz* k_e = StworzLokalnaMacierzSztywnosci(e);
	KWektor* p_e = StworzLokalnyWektorNaprezen(e);

	unsigned globalny_nr;
	float wyraz_warI;

	// okresl ktore wezly w elemencie sa warunkami brzegowymi I rodzaju
	vector<unsigned> wezly_war_I; //wektor numerow wezlow elementu bedacych warunkami I rodzaju
	vector<int> il_wezlow_war_I_przed;
	for (int i(1); i <= e->PobierzWezly()->size(); ++i) // iteruj po wezlach elementu e
	{
		for (auto it_warI : *dane->PobierzWarunkiI()) // iteruj po globalnych warunkach I
		{
			if (e->PobierzWezel(i)->PobierzX() == it_warI.x) // jesli i-ty wezel elementu jest warunkiem brzegowym
				wezly_war_I.push_back(i);

			// okresl ile wezlow o mniejszych numerach w pozostalych elementach sa warunkami I rodzaju; sluzy do zmiany indeksow globalnego zredukowanego ukladu rownan
			il_wezlow_war_I_przed.push_back(0);
			if (it_warI.pw->PobierzNumer() < e->PobierzWezel(i)->PobierzNumer())
				il_wezlow_war_I_przed[i - 1] += 1;
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
	for (unsigned i = 1; i <= p_e->DajN(); i++)
	{
		wyraz_warI = 0;
		if (any_of(wezly_war_I.begin(), wezly_war_I.end(), [&](const int& ity) { return ity == i; }))
			continue;
		globalny_nr = e->PobierzWezel(i)->PobierzNumer() - il_wezlow_war_I_przed[i - 1];
		for (auto it_wI : wezly_war_I)
		{
			wyraz_warI += (*k_e)(i, it_wI) * dane->PobierzSiatke()->PobierzWezel(globalny_nr)->PobierzTemperature();
		}
		(*urMES->PobierzY())(globalny_nr) += p_e->DajIJ(i) - wyraz_warI;
	}

	delete k_e;
	delete p_e;
}

void KObliczenia::WypelnijKWarunkamiBrzegowymi()
{
	WypelnijWarunkamiIRodzaju();
	WypelnijWarunkamiIIRodzaju();
}


KWektor* KObliczenia::Licz(KDane* idane)
{
	if (dane) delete dane;
	dane = idane;

	// tworzenie i wypelnianie ukladu rownan
	unsigned il_warunkow_I = dane->PobierzWarunkiI()->size();
	unsigned rozmiar = (unsigned)dane->PobierzSiatke()->vpWezly.size() - il_warunkow_I; // rozmiar ukladu rownan = ilosc wezlow - ilosc waunkow I rozadju
	urMES = new KUkladRownan(rozmiar);

	// iteruj po wszystkich elementach i uzupelnij globalna macierz sztywnosci macierzami lokalnymi
	for (auto it_e =  dane->PobierzSiatke()->vElementy.begin(); it_e != dane->PobierzSiatke()->vElementy.end(); ++it_e)
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
	WypelnijWarunkamiKonwekcyjnymi(urMES);

	// rozwiaz uklad rownan

	return urMES->PobierzX();
}

void KObliczenia::WypelnijWarunkamiIRodzaju()
{
	for (auto it_warI = dane->PobierzWarunkiI()->begin(); it_warI != dane->PobierzWarunkiI()->begin(); ++it_warI)
	{
		
	}
}

void KObliczenia::WypelnijWarunkamiIIRodzaju()
{

}

void KObliczenia::WypelnijWarunkamiKonwekcyjnymi(KUkladRownan* ur)
{
	unsigned il_wezlow_z_war_I;
	float h, T_inf;


	for (auto it_wk : *dane->PobierzWarunkiKonwekcyjne())
	{
		il_wezlow_z_war_I = 0;

		// sprawdz ile wezlow z warunkami I rodzaju  znajduje sie przed wezlem z warunkiem konwekcyjnym
		for (auto it_wI : *dane->PobierzWarunkiI())
		{
				if (it_wI.pw->PobierzNumer() < it_wk.wk->PobierzNumer()) // jesli i-ty wezel jest warunkiem brzegowym
					il_wezlow_z_war_I++;
		}

		// wypelnij globalny wektor naprezen warunkiem
		h = it_wk.h;
		T_inf = it_wk.T_inf;
		(*ur->PobierzY())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += h * T_inf;
		(*ur->PobierzA())(it_wk.wk->PobierzNumer() - il_wezlow_z_war_I, it_wk.wk->PobierzNumer() - il_wezlow_z_war_I) += it_wk.h ;
	}
}

void KObliczenia::WypelnijZrodlamiPunktowymi(KUkladRownan* ur)
{
	unsigned il_wezlow_z_war_I;

	for (auto it_zp : *dane->PobierzZrodlaPunktowe())
	{
		il_wezlow_z_war_I = 0;
		for (auto it_wI : *dane->PobierzWarunkiI())
		{
			for (auto it_warI : *dane->PobierzWarunkiI())
			{
				if (it_wI.x == it_warI.x) // jesli i-ty wezel jest warunkiem brzegowym
					il_wezlow_z_war_I++;
			}
		}
		(*ur->PobierzY())(it_zp.pz->PobierzNumer() - il_wezlow_z_war_I) += it_zp.g;
	}
}

void KObliczenia::WypelnijZrodlamiRozciaglymi()
{
}

KMacierz* KObliczenia::PobierzGlobalnaMacierzSztywnosci()
{
	return urMES->PobierzA();
}

KWektor* KObliczenia::PobierzGlobalnyWektorNaprezen()
{
	return urMES->PobierzY();
}
