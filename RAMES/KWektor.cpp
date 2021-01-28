#include "pch.h"
#include "KWektor.h"

/////////////////////////////////////////////
// Tworzy wektor in-elementowy wypelniony zerami

KWektor::KWektor(unsigned in)
{
    n = in;
	Stworz();
	Zeros();
}

KWektor::~KWektor()
{
	KMacierz::~KMacierz();
}

float KWektor::operator()(unsigned i) {
	return A[1][i];
}
