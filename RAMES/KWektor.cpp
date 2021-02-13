#include "pch.h"
#include "KWektor.h"

#include <stdexcept>

/////////////////////////////////////////////
// KWektor

double KWektor::DajI(int i) const
{
    return M[i - 1];
}

KWektor& KWektor::operator=(double* tablica1D)
{
    for (unsigned i = 0; i < mRozmiar; i++)
    {
        this->M[i] = (tablica1D)[i];
    }
    return *this;
}

double& KWektor::operator()(unsigned i)
{
    if (i > mRozmiar || i < 1)
        throw std::runtime_error("Indeks poza zakresem");

    return this->M[i - 1];
}

double KWektor::operator()(unsigned i) const
{
    if (i > mRozmiar || i < 1)
        throw std::runtime_error("Indeks poza zakresem");

    return this->M[i - 1];
}

/////////////////////////////////////////////
// KWektorW

KWektorW::KWektorW(unsigned in, const double* tablica): KWektor(m, in)
{
    for (unsigned i(0); i < mRozmiar; ++i)
            M[i] = *(tablica + i);
}

/////////////////////////////////////////////
// KWektorK

KWektorK::KWektorK(unsigned im, const double* tablica) : KWektor(im, n)
{
    for (unsigned i(0); i < mRozmiar; ++i)
        M[i] = *(tablica + i);
}

KWektorK::KWektorK(KMacierz& iM): KWektor(0, 1)
{
    if (iM.DajN() != 1)
        throw std::runtime_error("macierz ma wiecej niz jedna kolumne");

    m = iM.DajM();
    Stworz();
    for (unsigned i(0); i < mRozmiar; ++i)
        M[i] = iM.DajIJ(i + 1, 1);
}
