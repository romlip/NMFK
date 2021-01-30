
#pragma once
#include <vector>

#include "KDane.h"
#include "KMacierz.h"
#include "KWektor.h"
#include "KUkladRownan.h"
#include "KElement1D.h"

class KObliczenia
{
private:
	short int n; //liczba wezlow w elemencie
	KDane* dane;
	KUkladRownan* urMES;
	std::vector<KMacierz> k; // wektor lokalnych macierzy sztywnosci elementow
	std::vector<KWektor> p; // wektor lokalnych wektorow naprezen elementow

	KMacierz* StworzLokalnaMacierzSztywnosci(KElement1D* element);
	KWektor* StworzLokalnyWektorNaprezen(KElement1D* element);
	void WypelnijK(KElement1D* e);
	void WypelnijP(KElement1D* e);
	void WypelnijKWarunkamiBrzegowymi();

public:
	KObliczenia();
	~KObliczenia();

	KMacierz* PobierzGlobalnaMacierzSztywnosci();
	KWektor* Licz(KDane* idane);
};
