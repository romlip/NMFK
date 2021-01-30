#include "pch.h"
#include "KWektor.h"
#include <stdexcept>

/////////////////////////////////////////////
// Tworzy wektor in-elementowy wypelniony zerami

KWektor::KWektor()
{
}

KWektor::KWektor(unsigned in): KMacierz(m, in)
{
    n = in;
}

KWektor::~KWektor()
{
    if (A) delete[] A;
}

KWektor& KWektor::operator=(float* tablica1D)
{
    for (unsigned i = 0; i < n; i++)
    {
            this->A[i] = (tablica1D)[i];
    }
    return *this;
}

float& KWektor::operator()(unsigned i)
{
    if (i > n || i < 1)
    {
        throw std::runtime_error("Indeks poza zakresem");
    }
    return A[i-1];
}
