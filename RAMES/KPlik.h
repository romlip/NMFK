#pragma once

#include "KDane.h"
#include "KObliczenia.h"

class KPlik
{
private:
	char* nazwaPliku;
	enum enum_spec { WARUNKI_BRZEGOWE_I_RODZAJU = 1, ZRODLA_PUNKTOWE = 2, STRUKTURA = 3 }
		specyfikatory;
	const char* start = "START";
	const char* koniec = "KONIEC";

public:
	KPlik();
	KPlik(char* inazwaPliku);
	~KPlik();

	int SzukajSpecyfikatora(std::ifstream& plik);
	int WczytajDane(char* inazwaPliku, KDane* dane);
	void CzytajWarunkiI(std::ifstream& plik, KDane* dane);
	void CzytajZrodlaPunktowe(std::ifstream& plik, KDane* dane);
	void CzytajStrukture(std::ifstream& plik, KDane* dane);
	void ZapiszWynik(char* inazwaPliku, KDane* dane, KObliczenia* obliczenia);
};

