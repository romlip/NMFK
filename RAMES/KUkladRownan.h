#pragma once

#include "KMacierz.h"
#include "KWektor.h"

// uklad rownan postaci A * X = Y
// rozwiazaniem jest wektor X = Y * A^-1

class KUkladRownan
{
private:
	KMacierz* A; //macierz wspolczynnikow
	KWektor* Y; // wektor wyrazow wolnych
	KWektor* LiczXgradientySprzezone();

public:
	KWektor* X; // wektor niewiadomych

	KUkladRownan();
	~KUkladRownan();

	void UstawA(KMacierz* iA);
	void UstawY(KWektor* iY);

	KWektor* LiczX(const char* imetoda);
	KWektor* LiczX(KMacierz* iA, KWektor* iY, const char* imetoda);


};

