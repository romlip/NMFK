#include "pch.h"
#include "KMacierzSPF.h"

KMacierzSPF::KMacierzSPF() : KMacierz()
{

}

KMacierzSPF::KMacierzSPF(unsigned rozmiar, unsigned szerokoscPasma) : KMacierz(rozmiar, szerokoscPasma)
{
}

KMacierzSPF::KMacierzSPF(const KMacierzSPF& copy)
{
    m = copy.m;
    n = copy.n;
    M = new double[static_cast<unsigned long long>(m) * n];
    for (unsigned i(0); i < m; ++i)
        for (unsigned j(0); j < n; ++j)
            M[i * n + j] = copy.M[i * n + j];
}
