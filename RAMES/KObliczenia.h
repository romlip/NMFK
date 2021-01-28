#pragma once
#include <vector>

#include "KDane.h"
#include "KMacierz.h"
#include "KWektor.h"
#include "KUkladRownan.h"

class KObliczenia
{
private:
	KUkladRownan* urMES;
	std::vector<KMacierz> k; // wektor lokalnych macierzy sztywnosci elementow
	std::vector<KWektor> p; // wektor lokalnych wektorow naprezen elementow

public:
	KObliczenia();
	~KObliczenia();

	KWektor* Licz(KDane* dane);
};
