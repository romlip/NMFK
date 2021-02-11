#pragma once

#include "KMacierz.h"

class KWektor :
    public KMacierz
{
protected:

public:
    KWektor() {};
    KWektor(unsigned im, unsigned in) : KMacierz(im, in) {};
    ~KWektor() {};

    virtual KWektor& operator=(double* const tablica1D);
    virtual double& operator()(unsigned i);

    double DajI(int i);
};

class KWektorW :
    public KWektor
{
protected:
    static const unsigned m = 1; // 1 wiersz

public:
    KWektorW() : KWektor() {};
    KWektorW(unsigned in) : KWektor(m, in) {};
    KWektorW(unsigned in, const double* tablica);
    ~KWektorW() {};
};

class KWektorK :
    public KWektor
{
protected:
    static const unsigned n = 1; // 1 kolumna

public:
    //KWektorK() : KWektor() {};
    KWektorK(unsigned im) : KWektor(im, n) {};
    KWektorK(unsigned im, const double* tablica);
    KWektorK(KMacierz &iM);

    ~KWektorK() {};
};


