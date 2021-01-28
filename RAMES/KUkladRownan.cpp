#include "pch.h"
#include "KUkladRownan.h"
#include <iostream>

using namespace std;

KUkladRownan::KUkladRownan()
{
	A = nullptr;
	Y = nullptr;
	X = nullptr;
}

KUkladRownan::~KUkladRownan()
{
	if (A) delete A;
	if (Y) delete Y;
	if (X) delete X;
}

void KUkladRownan::UstawA(KMacierz* iA)
{
	A = iA;
}

void KUkladRownan::UstawY(KWektor* iY)
{
	Y = iY;
}

KWektor* KUkladRownan::LiczX(const char* imetoda)
{
	if (!A || !Y)
	{
		cerr << "Blont! Nie ustawiono macierzy A i/lub wektora Y\n";
			return nullptr;
	}

	if (imetoda == "gradienty_sprzezone")
		return LiczXgradientySprzezone();
}

KWektor* KUkladRownan::LiczX(KMacierz* iA, KWektor* iY, const char* imetoda)
{
	UstawA(iA);
	UstawY(iY);
	return LiczX(imetoda);
}

KWektor* KUkladRownan::LiczXgradientySprzezone()
{
	// cialo liczace uklad gradyjentami spszenrzonymi

	return X;
}
