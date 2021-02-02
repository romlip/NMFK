#include "pch.h"
#include "KPlik.h"
#include "KMacierz.h"
#include "KWektor.h"

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

int KPlik::SzukajSpecyfikatora(ifstream& plik, string &line)
{
	string sspecyfikatory[] = {"STRUKTURA", "WARUNKI_BRZEGOWE_I_RODZAJU", "WARUNKI_BRZEGOWE_II_RODZAJU", "WARUNKI_KONWEKCYJNE", "ZRODLA_PUNKTOWE", "ZRODLA_ROZCIAGLE", "LICZBA_WEZLOW_W_ELEMENCIE", "SKALA" };

	while (getline(plik, line) && plik.good())
	{
		for(int i = 0; i < sizeof(sspecyfikatory)/sizeof(sspecyfikatory[0]); i++)
		{
			if (line.find(sspecyfikatory[i]) != string::npos)
			{
				return i;
			}
		}
	}
	return -1;
}

int KPlik::WczytajDane(char* inazwaPliku, KDane* dane)
{
	ifstream pliczek(inazwaPliku, ios::in);
	if (!pliczek.good())
	{
		throw runtime_error("Cos jest nie tak z plikiem");
		return 1;
	}
	string line;

	while (pliczek.good())
	{
		switch (SzukajSpecyfikatora(pliczek, line))
		{
		case STRUKTURA:
			CzytajStrukture(pliczek, dane);
		case WARUNKI_BRZEGOWE_I_RODZAJU:
			CzytajWarunkiI(pliczek, dane);
			break;
		case WARUNKI_BRZEGOWE_II_RODZAJU:
			CzytajWarunkiII(pliczek, dane);
			break;
		case WARUNKI_KONWEKCYJNE:
			CzytajWarunkiKonwekcyjne(pliczek, dane);
			break;
		case ZRODLA_PUNKTOWE:
			CzytajZrodlaPunktowe(pliczek, dane);
			break;
		case ZRODLA_ROZCIAGLE:
			CzytajZrodlaRozciagle(pliczek, dane);
			break;
		case LICZBA_WEZLOW:
			CzytajLiczbeWezlow(pliczek, dane, line);
			break;
		default:
			break;
		}
	}
	dane->FinalizujWczytywanie();

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
		strukt_warunekI warunek = { x, T, nullptr };
		dane->DodajWarunekI(warunek);
	}
}

void KPlik::CzytajWarunkiII(std::ifstream& plik, KDane* dane)
{
	string line;
	float x, q;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START

	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> q;
		strukt_warunekII warunek = { x, q };
		dane->DodajWarunekII(warunek);
	}
}

void KPlik::CzytajWarunkiKonwekcyjne(std::ifstream& plik, KDane* dane)
{
	string line;
	float x, h, T_inf;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START

	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> h >> T_inf;
		strukt_warunek_konwekcyjny warunek = { x, h, T_inf };
		dane->DodajWarunekKonwekcyjny(warunek);
	}
}

void KPlik::CzytajZrodlaPunktowe(ifstream& plik, KDane* dane)
{
	string line;
	float x, g;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START

	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> g;
		strukt_zrodlo_punktowe zrodlo = { x, g, nullptr };
		dane->DodajZrodloPunktowe(zrodlo);
	}
}

void KPlik::CzytajZrodlaRozciagle(std::ifstream& plik, KDane* dane)
{
	string line;
	float x, g;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START

	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> g;
		strukt_zrodlo_rozciagle zrodlo = { x, g }; // [g] = [W/m^2]
		dane->DodajZrodloRozciagle(zrodlo);
	}

}

void KPlik::CzytajStrukture(ifstream& plik, KDane* dane)
{
	string line;
	unsigned _nr;
	float _xl, _xp, _k, _f;

	while (getline(plik, line) && line.find(start) == string::npos && plik.good()) {} //szukamy START

	while (getline(plik, line) && line.find(koniec) == string::npos && plik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> _nr >> _xl >> _xp >> _k >> _f;
		dane->siatka->DodajElement(_nr, _xl, _xp, _k, _f);
	}
}

void KPlik::CzytajLiczbeWezlow(ifstream& plik, KDane* dane, string& line)
{
	stringstream sline(line);
	int liczba_wezlow;
	string specyf;
	sline >> specyf >> liczba_wezlow;
	dane->PobierzSiatke()->UstawLiczbeWezlowWelemencie(liczba_wezlow);
}

void KPlik::ZapiszWynik(char* inazwaPliku, KDane* dane, KObliczenia* obliczenia)
{
	ofstream plik(inazwaPliku, ios::out);
	if (plik.good())
	{
		plik << "wezly\n";
		for (auto it_wezly = dane->PobierzSiatke()->vpWezly.begin(); it_wezly != dane->siatka->vpWezly.end(); ++it_wezly)
		{
			plik <<(*it_wezly)->PobierzNumer() <<"\t"<<(*it_wezly)->PobierzX() << "\n";
		}

		plik << "elementy\n";
		for (auto it_elementy = dane->siatka->vElementy.begin(); it_elementy != dane->siatka->vElementy.end(); ++it_elementy)
		{
			plik << it_elementy->PobierzNumer() << "\t";
			for (auto it_wezly = it_elementy->PobierzWezly()->begin(); it_wezly != it_elementy->PobierzWezly()->end(); ++it_wezly)
				plik << (*it_wezly)->PobierzX() << "\t";
			plik << it_elementy->Pobierzk() / it_elementy->Pobierzh() << "\n";
		}
	}
	plik << "\nmacierz sztywnosci:\n";

	KMacierz* K = obliczenia->PobierzGlobalnaMacierzSztywnosci();
	K->Wypisz(plik);

	plik << "\nwektor naprezen:\n";
	KWektor* P = obliczenia->PobierzGlobalnyWektorNaprezen();
	P->Wypisz(plik);

}


