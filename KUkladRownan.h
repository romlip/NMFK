#pragma once

#include "KMacierz.h"
#include "KWektor.h"

// uklad rownan postaci A * X = Y
// rozwiazaniem jest wektor X = Y * A^-1

class KUkladRownan
{
private:
	KMacierz* A; //macierz wspolczynnikow
	KWektorK* B; // wektor wyrazow wolnych
	KWektorK* X; // wektor niewiadomych

	KWektorK* RozwiazGradientamiSprzezonymi();
	KWektorK* RozwiazNadrelaksacja();
	KWektorK* RozwiazCholesky();
	KWektorK* RozwiazEliminacja(int gorna_dolna);

public:

	KUkladRownan();
	//KUkladRownan(unsigned ili_wezlow);
	KUkladRownan(KMacierz *A, KWektorK* iX, KWektorK* B);
	~KUkladRownan();

	KMacierz* PobierzA();
	KWektorK* PobierzB();
	KWektorK* PobierzX();
	void UstawA(KMacierz* iA);
	void UstawB(KWektorK* iB);

	KWektorK* Rozwiaz(const char* imetoda = "cholesky");
	KWektorK* Rozwiaz(KMacierz* iA, KWektorK* iY, const char* imetoda);

	KWektorK f(KMacierz& B, KWektorK& c, KWektorK& x0);

};

