#include "pch.h"
#include "KWektor.h"

#include <stdexcept>

/////////////////////////////////////////////
// KWektor

float KWektor::DajI(int i)
{
    return M[i - 1];
}

KWektor& KWektor::operator=(float* tablica1D)
{
    for (unsigned i = 0; i < m * n; i++)
    {
        this->M[i] = (tablica1D)[i];
    }
    return *this;
}

float& KWektor::operator()(unsigned i)
{
    if (i > m * n || i < 1)
    {
        throw std::runtime_error("Indeks poza zakresem");
    }
    return this->M[i - 1];
}

/////////////////////////////////////////////
// KWektorW

KWektorW::KWektorW(unsigned in, const float* tablica): KWektor(m, in)
{
    for (unsigned i(0); i < in; ++i)
    {
            M[i] = *(tablica + i);
    }
}

/////////////////////////////////////////////
// KWektorK

KWektorK::KWektorK(unsigned im, const float* tablica) : KWektor(im, n)
{
    for (unsigned i(0); i < im; ++i)
    {
        M[i] = *(tablica + i);
    }
}

KWektorK::KWektorK(KMacierz& iM): KWektor(0, 1)
{
    if (iM.DajN() != 1)
        throw std::runtime_error("macierz ma wiecej niz jedna kolumne");
    m = iM.DajM();
    Stworz();
    for (unsigned i(0); i < m; ++i)
        M[i] = iM.DajIJ(i + 1, 1);
}
