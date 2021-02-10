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

    virtual KWektor& operator=(float* const tablica1D);
    virtual float& operator()(unsigned i);

    float DajI(int i);
};

class KWektorW :
    public KWektor
{
protected:
    static const unsigned m = 1; // 1 wiersz

public:
    KWektorW() : KWektor() {};
    KWektorW(unsigned in) : KWektor(m, in) {};
    KWektorW(unsigned in, const float* tablica);
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
    KWektorK(unsigned im, const float* tablica);
    KWektorK(KMacierz &iM);

    ~KWektorK() {};
};


