#include "pch.h"
#include "KWektor.h"
#include <stdexcept>

/////////////////////////////////////////////
// Tworzy wektor in-elementowy wypelniony zerami

KWektor::KWektor(): KMacierz()
{
}

KWektor::KWektor(unsigned in): KMacierz(m, in)
{
}

KWektor::KWektor(unsigned in, const float* tablica): KMacierz(m, in)
{
    for (unsigned i(0); i < in; ++i)
    {
            A[i] = *(tablica + i);
    }
}

KWektor::~KWektor()
{
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
    return this->A[i-1];
}

float KWektor::DajIJ(int i)
{
    return A[i-1];
}
