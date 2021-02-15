#include "pch.h"
#include "KDane.h"

using namespace std;

KDane::KDane()
{
	Inicjalizuj();
}

KDane::~KDane()
{
}

void KDane::UstawSkale(double iSkala)
{
	mfSkala = iSkala;
}

void KDane::ZastosujSkale()
{
	for (auto it_w = mSiatka.PobierzWezly()->begin(); it_w != mSiatka.PobierzWezly()->begin(); ++it_w)
	{
		(*it_w)->UstawX((*it_w)->PobierzX() * mfSkala);
	}
}

void KDane::OdstosujSkale()
{
	for (auto it_w = mSiatka.PobierzWezly()->begin(); it_w != mSiatka.PobierzWezly()->begin(); ++it_w)
	{
		(*it_w)->UstawX((*it_w)->PobierzX() / mfSkala);
	}
}

//////////////////////////////
// Dodaje wskazniki wezlow warunku I w wektorze warunkiI

void KDane::DodajWezlyWarI()
{
	for (auto it_wI = warunkiI.begin(); it_wI != warunkiI.end(); ++it_wI)
	{
		for (auto it_w = mSiatka.PobierzWezly()->begin(); it_w != mSiatka.PobierzWezly()->end(); ++it_w)
		{
			if ((*it_w)->PobierzX() == it_wI->x)
			{
				(*it_w)->UstawTemperature(it_wI->T);
				it_wI->pw = *it_w;
			}
		}
	}
}

void KDane::DodajWezlyWarII()
{
	for (auto it_wii = warunkiII.begin(); it_wii != warunkiII.end(); ++it_wii)
	{
		for (auto it_w = mSiatka.PobierzWezly()->begin(); it_w != mSiatka.PobierzWezly()->end(); ++it_w)
		{
			if ((*it_w)->PobierzX() == it_wii->x)
			{
				it_wii->pWii = *it_w;
			}
		}
	}

}

void KDane::DodajWezlyWarunkowBrzegowych()
{
	DodajWezlyWarI();
	DodajWezlyWarII();
	DodajWezlyWarKon();
}

void KDane::WyczyscDane()
{
	// siatka.clear();
	warunkiI.clear();
	warunkiII.clear();
	warunki_konwekcyjne.clear();
	zrodla_punktowe.clear();
	zrodla_rozciagle.clear();
}

void KDane::DodajWezlyWarKon()
{
	for (auto it_wk = warunki_konwekcyjne.begin(); it_wk != warunki_konwekcyjne.end(); ++it_wk)
	{
		for (auto it_w = mSiatka.PobierzWezly()->begin(); it_w != mSiatka.PobierzWezly()->end(); ++it_w)
		{
			if ((*it_w)->PobierzX() == it_wk->x)
			{
				it_wk->wk = *it_w;
			}
		}
	}
}

void KDane::DodajWarunekI(strukt_warunekI& warunekI)
{
	warunkiI.push_back(warunekI);
}

void KDane::DodajWarunekII( strukt_warunekII& warunekII)
{
	warunkiII.push_back(warunekII);
}

void KDane::DodajWarunekKonwekcyjny(strukt_warunek_konwekcyjny& warunek_konwekcyjny)
{
	warunki_konwekcyjne.push_back(warunek_konwekcyjny);
}

void KDane::DodajZrodloRozciagle(strukt_zrodlo_rozciagle& zrodlo_rozciagle)
{
	zrodla_rozciagle.push_back(zrodlo_rozciagle);
}

void KDane::SprawdzPoprawnoscDanych()
{
	// sprawdz czy struktura sie wczytala
	if (mSiatka.PobierzElementy()->empty())
	{
		WyczyscDane();
		throw runtime_error("Nie udalo sie wczytac struktury.");
	}

	// sprawdz czy wspolredne warunkow brzegowych i punktow zrodlowych mieszcza sie w strukturze


	//sprawdz spojnosc obszarow
	for (auto it_e = mSiatka.PobierzElementy()->begin(); it_e + 1 != mSiatka.PobierzElementy()->end(); ++it_e)
	{
		if ((it_e + 1)->PobierzWezel(1)->PobierzX() != (it_e)->PobierzWezel(2)->PobierzX())
		{
			char komunikat[50];
			sprintf_s(komunikat, "Obszary %d i %d nie sa spojne.", it_e->PobierzNumer(), (it_e + 1)->PobierzNumer());
			WyczyscDane();
			throw runtime_error(komunikat);
		}
	}

	// sprawdz czy nie ma wiecej niz 2 warunki brzegowe
	if (warunkiI.size() + warunkiII.size() + warunki_konwekcyjne.size() > 2)
	{
		WyczyscDane();
		throw runtime_error("Podano za duzo warunkow brzegowych. Dla struktury 1D nie moze byc ich wiecej niz 2.");
	}
}

KSiatka* KDane::PobierzSiatke()
{
	return &mSiatka;
}

void KDane::Inicjalizuj()
{
	mDaneFlag = false;
	mfSkala = 1;
}

void KDane::Wyczysc()
{
	warunkiI.clear();
	warunkiII.clear();
	warunki_konwekcyjne.clear();
	zrodla_punktowe.clear();
	zrodla_rozciagle.clear();
	mSiatka.Wyczysc();

	Inicjalizuj();
}

vector<strukt_warunekI>* KDane::PobierzWarunkiI()
{
	return &warunkiI;
}

 vector<strukt_warunekII>* KDane::PobierzWarunkiII()
{
	return &warunkiII;
}

vector<strukt_warunek_konwekcyjny>* KDane::PobierzWarunkiKonwekcyjne()
{
	return &warunki_konwekcyjne;
}

 vector<strukt_zrodlo_rozciagle>* KDane::PobierzZrodlaRozciagle()
{
	return &zrodla_rozciagle;
}

 vector<strukt_warunekI>* KDane::PobierzWezlyWarI()
 {
	 return &warunkiI;
 }

 void KDane::FinalizujWczytywanie()
 {
	 //ZastosujSkale();
	 mSiatka.Finalizuj();
	 DodajWezlyWarunkowBrzegowych();// dodaj wskazniki na wezly warunkow brzegowych
	 SprawdzPoprawnoscDanych();

	 mDaneFlag = true;
 }