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
	Inicjalizuj();
}

KPlik::~KPlik()
{
}

///////////////////////////////////////////////////////////////////////////////
// wczytuje plik ze struktura i zwraca wskaznik na utworzona w ten sposob siatke

int KPlik::SzukajSpecyfikatora(string &line)
{
	string sspecyfikatory[] = {"STRUKTURA", "WARUNKI_BRZEGOWE_I_RODZAJU", "WARUNKI_BRZEGOWE_II_RODZAJU", "WARUNKI_KONWEKCYJNE", "ZRODLA_PUNKTOWE", "ZRODLA_ROZCIAGLE", "LICZBA_WEZLOW_W_ELEMENCIE", "NAZWA", "SKALA" };

	while (getline(mPlik, line) && mPlik.good())
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

int KPlik::WczytajDane(char* inazwaPliku, KDane* pDane)
{
	UstawDane(pDane);

	mPlik.open(inazwaPliku, ios::in);
	if (!mPlik.good())
	{
		throw runtime_error("Cos jest nie tak z plikiem");
		return 1;
	}
	mpDane->Wyczysc();

	string line;

	while (mPlik.good())
	{
		switch (SzukajSpecyfikatora(line))
		{
		case STRUKTURA:
			CzytajStrukture();
		case WARUNKI_BRZEGOWE_I_RODZAJU:
			CzytajWarunkiI();
			break;
		case WARUNKI_BRZEGOWE_II_RODZAJU:
			CzytajWarunkiII();
			break;
		case WARUNKI_KONWEKCYJNE:
			CzytajWarunkiKonwekcyjne();
			break;
		case ZRODLA_PUNKTOWE:
			CzytajZrodlaPunktowe();
			break;
		case ZRODLA_ROZCIAGLE:
			CzytajZrodlaRozciagle();
			break;
		case LICZBA_WEZLOW:
			CzytajLiczbeWezlow(line);
			break;
		case NAZWA:
			CzytajNazwe(line);
			break;
		case SKALA:
			CzytajSkale(line);
		default:
			break;
		}
	}
	try {
		mpDane->FinalizujWczytywanie();
	}
	catch (exception& e)
	{
		MessageBox(NULL, CString(e.what()), _T("Blad wczytywania danych"), MB_OK | MB_ICONWARNING);
		mpDane->mDaneFlag = false;
	}

	mPlik.close();

	return 0;
}

void KPlik::UstawDane(KDane* pDane)
{
	mpDane = pDane;
}

void KPlik::CzytajWarunkiI()
{
	string line;
	double x, T;

	while (getline(mPlik, line) && line.find(start) == string::npos && mPlik.good()) {} //szukamy START
	
	while (getline(mPlik, line) && line.find(koniec) == string::npos && mPlik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> T;
		strukt_warunekI warunek = { x, T, nullptr };
		mpDane->DodajWarunekI(warunek);
	}
}

void KPlik::CzytajWarunkiII()
{
	string line;
	double x, q;

	while (getline(mPlik, line) && line.find(start) == string::npos && mPlik.good()) {} //szukamy START

	while (getline(mPlik, line) && line.find(koniec) == string::npos && mPlik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> q;
		strukt_warunekII warunek = { x, q };
		mpDane->DodajWarunekII(warunek);
	}
}

void KPlik::CzytajWarunkiKonwekcyjne()
{
	string line;
	double x, h, T_inf;

	while (getline(mPlik, line) && line.find(start) == string::npos && mPlik.good()) {} //szukamy START

	while (getline(mPlik, line) && line.find(koniec) == string::npos && mPlik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> h >> T_inf;
		strukt_warunek_konwekcyjny warunek = { x, h, T_inf };
		mpDane->DodajWarunekKonwekcyjny(warunek);
	}
}

void KPlik::CzytajZrodlaPunktowe()
{
	string line;
	double x, g;

	while (getline(mPlik, line) && line.find(start) == string::npos && mPlik.good()) {} //szukamy START

	while (getline(mPlik, line) && line.find(koniec) == string::npos && mPlik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> g;
		strukt_zrodlo_punktowe zrodlo = { x, g, nullptr };
		mpDane->PobierzSiatke()->DodajZrodloPunktowe(zrodlo);
	}
}

void KPlik::CzytajZrodlaRozciagle()
{
	string line;
	double x, g;

	while (getline(mPlik, line) && line.find(start) == string::npos && mPlik.good()) {} //szukamy START

	while (getline(mPlik, line) && line.find(koniec) == string::npos && mPlik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> x >> g;
		strukt_zrodlo_rozciagle zrodlo = { x, g }; // [g] = [W/m^2]
		mpDane->DodajZrodloRozciagle(zrodlo);
	}
}

void KPlik::CzytajStrukture()
{
	string line;
	unsigned _nr;
	double xl, xp, _k, _f;

	while (getline(mPlik, line) && line.find(start) == string::npos && mPlik.good()) {} //szukamy START

	while (getline(mPlik, line) && line.find(koniec) == string::npos && mPlik.good()) // czytamy warunki az do KONIEC
	{
		stringstream sline(line);
		sline >> _nr >> xl >> xp >> _k >> _f;

		if (xl < mpDane->PobierzSiatke()->PobierzXmin())
			mpDane->PobierzSiatke()->UstawXmin(xl);
		if (xp > mpDane->PobierzSiatke()->PobierzXmax())
			mpDane->PobierzSiatke()->UstawXmax(xp);

		mpDane->PobierzSiatke()->DodajElement(_nr, xl, xp, _k, _f);
	}
}

void KPlik::CzytajLiczbeWezlow(string& line)
{
	stringstream sline(line);
	int liczba_wezlow;
	string specyf;
	sline >> specyf >> liczba_wezlow;
	mpDane->PobierzSiatke()->UstawLiczbeWezlowWelemencie(liczba_wezlow);
}

void KPlik::CzytajNazwe(string& line)
{
	stringstream sline(line);
	string specyf;
	sline >> specyf >> nazwaStruktury;
}

void KPlik::CzytajSkale(string& line)
{
	stringstream sline(line);
	string specyf;
	double skala;
	sline >> specyf >> skala;
	mpDane->UstawSkale(skala);
}

void KPlik::ZapiszWynik(const KObliczenia* obliczenia, const char* inazwaPliku)
{	
	string wynikiPath;
	if (inazwaPliku == "")
		wynikiPath = wynikiDir + nazwaStruktury + ".txt";
	else
		wynikiPath = inazwaPliku;

	mPlik.open(wynikiPath, ios::out);

	if (mPlik.good())
		obliczenia->WypiszWynik(mPlik);

	mPlik.close();
}

string* KPlik::PobierzNazwe()
{
	return &nazwaStruktury;
}

void KPlik::Inicjalizuj()
{
	mpDane = nullptr;
	nazwaPliku = "";
	specyfikatory = NAZWA;
}


