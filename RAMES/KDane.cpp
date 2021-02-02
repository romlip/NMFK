#include "pch.h"
#include "KDane.h"

using namespace std;

KDane::KDane()
{
	siatka = new KSiatka();
}

KDane::~KDane()
{
	if (siatka) delete siatka;
}

void KDane::DodajPunktyZrodlowe()
{
	// stworz wektor wskaznikow na wezly bedace zrodlami punktowymi; sluzy do wypelnienia globalnego wektora naprezen
	for (auto it_zp = zrodla_punktowe.begin(); it_zp != zrodla_punktowe.end(); ++it_zp)
	{
		float x_zrodla = it_zp->x;
		KWezel1D* punkt_zrodlowy = siatka->WezelIstnieje(x_zrodla);
		if (punkt_zrodlowy == nullptr)
		{
			punkt_zrodlowy = siatka->DodajWezel(x_zrodla); // stworz nowy wezel w x
			siatka->WstawElementZa(punkt_zrodlowy);
		}
		it_zp->pz = punkt_zrodlowy;
	}
}
//////////////////////////////
// Dodaje wskazniki wezlow warunku I w wektorze warunkiI

void KDane::DodajWezlyWarI()
{
	for (auto it_wI = warunkiI.begin(); it_wI != warunkiI.end(); ++it_wI)
	{
		for (auto it_w = siatka->PobierzWezly()->begin(); it_w != siatka->PobierzWezly()->end(); ++it_w)
		{
			if ((*it_w)->PobierzX() == it_wI->x)
			{
				(*it_w)->UstawTemperature(it_wI->T);
				it_wI->pw = *it_w;
			}
		}
	}
}

void KDane::DodajWezlyWarKon()
{
	for (auto it_wk = warunki_konwekcyjne.begin(); it_wk != warunki_konwekcyjne.end(); ++it_wk)
	{
		for (auto it_w = siatka->PobierzWezly()->begin(); it_w != siatka->PobierzWezly()->end(); ++it_w)
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

void KDane::DodajZrodloPunktowe( strukt_zrodlo_punktowe& zrodlo)
{
	zrodla_punktowe.push_back(zrodlo);
}

void KDane::DodajZrodloRozciagle(strukt_zrodlo_rozciagle& zrodlo_rozciagle)
{
	zrodla_rozciagle.push_back(zrodlo_rozciagle);
}

KSiatka* KDane::PobierzSiatke()
{
	return siatka;
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

 vector<strukt_zrodlo_punktowe>* KDane::PobierzZrodlaPunktowe()
{
	return &zrodla_punktowe;
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
	 // sprawdz czy struktura sie wczytala
	 if (siatka->PobierzWezly()->empty())
	 {
		 throw runtime_error("Nie wczytano struktury");
	 }
	 *siatka->PobierzStrukture() = *siatka->PobierzElementy(); 	 // zapisz wczytane elementy jako pierwotna strukture

	 DodajPunktyZrodlowe();	 // dodaj wezly w punktach zrodlowych i wumiesc ich wskazniki w wektorze zrodla_punktowe
	 DodajWezlyWarI(); // dodaj wskazniki wezlow warunku I w wektorze warunkiI
	 DodajWezlyWarKon();

	 siatka->NumerujWezly();

	 *siatka->PobierzWezlyWczytane() = *siatka->PobierzWezly(); // zapisz wczytane wezly i punkty zrodlowe jako pierwotne

 }