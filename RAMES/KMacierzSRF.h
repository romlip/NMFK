#pragma once
#include "KMacierz.h"

class KMacierzSRF :
    public KMacierz
{
private:

protected:
    unsigned* prow; // wskazuje na pierwszy element kazdego z wierszy
    unsigned** coln; // numery kolumn kolejnych wspolczynnikow
    //double **A; przechouje niezerowe wspolczynniki macierzy, jest dziedziczone z KMacierz

public:

};
