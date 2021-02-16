#pragma once

#include "KDane.h"
#include "KObliczenia.h"

class KPlik
{
private:
	KDane* mpDane;
	std::fstream mPlik;
	char* nazwaPliku;

	enum enum_spec { STRUKTURA , WARUNKI_BRZEGOWE_I_RODZAJU, WARUNKI_BRZEGOWE_II_RODZAJU, WARUNKI_KONWEKCYJNE, ZRODLA_PUNKTOWE, ZRODLA_ROZCIAGLE, LICZBA_WEZLOW, NAZWA, SKALA }
		specyfikatory;
	std::string wynikiDir = "Wyniki\\";
	const char* start = "START";
	const char* koniec = "KONIEC";

	int SzukajSpecyfikatora(std::string& line);
	void CzytajWarunkiI();
	void CzytajWarunkiII();
	void CzytajWarunkiKonwekcyjne();
	void CzytajZrodlaPunktowe();
	void CzytajZrodlaRozciagle();
	void CzytajStrukture();
	void CzytajLiczbeWezlow(std::string& line);
	void CzytajNazwe(std::string& line);
	void CzytajSkale(std::string& line);

public:
	std::string nazwaStruktury;

	KPlik();
	~KPlik();

	void UstawDane(KDane* pDane);
	int WczytajDane(char* inazwaPliku, KDane* dane);
	void ZapiszWynik(const KObliczenia* obliczenia, const char* inazwaPliku = "");
	std::string* PobierzNazwe();
	void Inicjalizuj();
};

