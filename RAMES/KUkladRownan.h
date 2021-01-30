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
	KWektor* X; // wektor niewiadomych

	KWektor* RozwiazGradientamiSprzezonymi();

public:

	KUkladRownan();
	~KUkladRownan();

	KMacierz* PobierzA();
	KWektor* PobierzY();
	KWektor* PobierzX();

	void UstawA(KMacierz* iA);
	void UstawY(KWektor* iY);

	KWektor* Rozwiaz(const char* imetoda);
	KWektor* Rozwiaz(KMacierz* iA, KWektor* iY, const char* imetoda);


};

