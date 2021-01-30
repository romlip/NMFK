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

KMacierz* KUkladRownan::PobierzA()
{
	return A;
}

KWektor* KUkladRownan::PobierzY()
{
	return Y;
}

KWektor* KUkladRownan::PobierzX()
{
	return X;
}

void KUkladRownan::UstawA(KMacierz* iA)
{
	A = iA;
}

void KUkladRownan::UstawY(KWektor* iY)
{
	Y = iY;
}

KWektor* KUkladRownan::Rozwiaz(const char* imetoda)
{
	if (!A || !Y)
	{
		cerr << "Blont! Nie ustawiono macierzy A i/lub wektora Y\n";
			return nullptr;
	}

	if (imetoda == "gradienty_sprzezone")
		return RozwiazGradientamiSprzezonymi();
}

KWektor* KUkladRownan::Rozwiaz(KMacierz* iA, KWektor* iY, const char* imetoda)
{
	UstawA(iA);
	UstawY(iY);
	return Rozwiaz(imetoda);
}

KWektor* KUkladRownan::RozwiazGradientamiSprzezonymi()
{
	// cialo liczace uklad gradyjentami spszenrzonymi

	return X;
}
