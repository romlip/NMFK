#include "pch.h"
#include "KPlik.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

KPlik::KPlik()
{
	nazwaPliku = "";
}

KPlik::KPlik(char* inazwaPliku)
{
	nazwaPliku = inazwaPliku;
}

KPlik::~KPlik()
{
}

/////////////////////////////////////////////////////////////////////////////
//wczytuje plik ze struktura i zwraca wskaznik na utworzona w ten sposob siatke

int KPlik::SzukajSpecyfikatora(ifstream& plik)
{
	string sspecyfikatory[] = { "WARUNKI_BRZEGOWE_I_RODZAJU", "ZRODLA_PUNKTOWE", "STRUKTURA" };
	string line;

	while (getline(plik, line) && plik.good())
	{
		for(int i = 0; i < sizeof(sspecyfikatory)/sizeof(sspecyfikatory[0]); i++)
		{
			if (line.find(sspecyfikatory[i]) != string::npos)
			{
				return WARUNKI_BRZEGOWE_I_RODZAJU + i;
			}
		}
	}
	return -1;
}

int KPlik::WczytajDane(char* inazwaPliku, KDane* dane)
{
	ifstream pliczek(inazwaPliku, ios::in);
	if (!pliczek.good())
		return 1;

	while (pliczek.good())
	{
		switch (SzukajSpecyfikatora(pliczek))
		{
		case WARUNKI_BRZEGOWE_I_RODZAJU:
			CzytajWarunkiI(pliczek, dane);
			break;
		case ZRODLA_PUNKTOWE:
			CzytajZrodlaPunktowe(pliczek, dane);
			break;
		case STRUKTURA:
			CzytajStrukture(pliczek, dane);
			break;
		default:
			break;
		}
	}
	return 0;
}

void KPlik::CzytajWarunkiI(ifstream& plik, KDane* dane)
{
	string line;
	float x, T;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START
	
	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> T;
		strukt_warunekI warunek = { x, T };
		dane->warunkiI.push_back(warunek);
	}
}

void KPlik::CzytajZrodlaPunktowe(ifstream& plik, KDane* dane)
{
	string line;
	float x, f;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START

	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> f;
		strukt_zrodlo_punktowe zrodlo = { x, f };
		dane->zrodla_punktowe.push_back(zrodlo);
	}
}

void KPlik::CzytajStrukture(ifstream& plik, KDane* dane)
{
	string line;
	unsigned _nr;
	float _xl, _xp, _k;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START

	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> _nr >> _xl >> _xp >> _k;
		dane->siatka->DodajElement(_nr, _xl, _xp, _k);
	}
}

void KPlik::ZapiszWynik(char* inazwaPliku, KDane* dane)
{
	ofstream plik(inazwaPliku, ios::out);
	if (plik.good())
	{
		plik << "wezly\n";
		for (auto it_wezly = dane->siatka->pWezly.begin(); it_wezly != dane->siatka->pWezly.end(); ++it_wezly)
		{
			plik <<(*it_wezly)->PobierzX() << "\n";
		}
		plik << "elementy\n";

		for (auto it_elementy = dane->siatka->elementy.begin(); it_elementy != dane->siatka->elementy.end(); ++it_elementy)
		{
			plik << it_elementy->PobierzNumer() <<"\t"<<it_elementy->PobierzLewy()->PobierzX() << "\t" << it_elementy->PobierzPrawy()->PobierzX() << "\n";
		}
	}
}
