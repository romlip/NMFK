#pragma once

#include "KMacierz.h"

class KWektor :
    public KMacierz
{
//    using KMacierz::KMacierz;

private:
    static const unsigned m = 1;

protected:

public:
    KWektor();
    KWektor(unsigned in);
    KWektor(unsigned in, const float* tablica);

    ~KWektor();

    KWektor& operator=(float* const tablica1D);
    virtual float& operator()(unsigned i);

    float DajIJ(int i);


};

