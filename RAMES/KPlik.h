#pragma once

#include "KDane.h"
#include "KObliczenia.h"

class KPlik
{
private:
	char* nazwaPliku;
	enum enum_spec { STRUKTURA , WARUNKI_BRZEGOWE_I_RODZAJU, WARUNKI_BRZEGOWE_II_RODZAJU, WARUNKI_KONWEKCYJNE, ZRODLA_PUNKTOWE, ZRODLA_ROZCIAGLE, LICZBA_WEZLOW, NAZWA, SKALA }
	specyfikatory;
	std::string wynikiDir = "Wyniki/";
	const char* start = "START";
	const char* koniec = "KONIEC";

public:
	std::string nazwaStruktury;

	KPlik();
	KPlik(char* inazwaPliku);
	~KPlik();

	int WczytajDane(char* inazwaPliku, KDane* dane);

	int SzukajSpecyfikatora(std::ifstream& plik, std::string &line);
	void CzytajWarunkiI(std::ifstream& plik, KDane* dane);
	void CzytajWarunkiII(std::ifstream& plik, KDane* dane);
	void CzytajWarunkiKonwekcyjne(std::ifstream& plik, KDane* dane);
	void CzytajZrodlaPunktowe(std::ifstream& plik, KDane* dane);
	void CzytajZrodlaRozciagle(std::ifstream& plik, KDane* dane);
	void CzytajStrukture(std::ifstream& plik, KDane* dane);
	void CzytajLiczbeWezlow(std::ifstream& plik, KDane* dane, std::string& line);
	void CzytajNazwe(std::ifstream& plik, KDane* dane, std::string& line);

	void ZapiszWynik(KDane* dane, KObliczenia* obliczenia, const char* inazwaPliku = "");
};

