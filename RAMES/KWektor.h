#pragma once

#include "KMacierz.h"

class KWektor :
    public KMacierz
{
protected:

public:
    KWektor() : KMacierz() {};
    KWektor(unsigned im, unsigned in) : KMacierz(im, in) {};
    ~KWektor() {};

    virtual KWektor& operator=(double* const tablica1D);
    virtual double& operator()(unsigned i);
    virtual double operator()(unsigned i) const;

    double DajI(int i) const;
};

class KWektorW :
    public KWektor
{
protected:
    static const unsigned m = 1; // 1 wiersz

public:
    KWektorW() : KWektor() {};
    KWektorW(unsigned in) : KWektor(1, in) {};
    KWektorW(unsigned in, const double* tablica);
    ~KWektorW() {};
};

class KWektorK :
    public KWektor
{
protected:
    static const unsigned n = 1; // 1 kolumna

public:
    KWektorK() : KWektor() {};
    KWektorK(unsigned im) : KWektor(im, 1) {};
    KWektorK(unsigned im, const double* tablica);
    KWektorK(KMacierz &iM);
    ~KWektorK() {};
};


