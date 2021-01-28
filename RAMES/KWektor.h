#pragma once
#include "KMacierz.h"

class KWektor :
    public KMacierz
{
private:
    static const unsigned m = 1;

protected:

public:
    KWektor(unsigned in);
    ~KWektor();

    virtual float operator()(unsigned i);
};

