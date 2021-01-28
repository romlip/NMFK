#include "pch.h"
#include "KMacierz.h"

void KMacierz::Stworz()
{
    A = new  float* [m];
    for (int i = 0; i < m; i++)
    {
        A[i] = new float[n];
    }
}

/////////////////////////////////////////////
// Zwraca element (i, j) macierzy

float KMacierz::operator()(unsigned i, unsigned j)
{
    return A[i][j];
}

KMacierz::KMacierz()
{
    m = 0;
    n = 0;
    A = nullptr;
}

/////////////////////////////////////////////
// Tworzy macierz kwadratowa in x in i wypelnia ja zerami

KMacierz::KMacierz(int in)
{
    m = in;
    n = in;
    Stworz();
    Zeros();
}

/////////////////////////////////////////////
// Tworzy macierz im x in i wypelnia ja zerami

KMacierz::KMacierz(int im, int in)
{
    m = in;
    n = in;
    Stworz();
    Zeros();
}


KMacierz::~KMacierz()
{
    if (A)
    {
        for (unsigned i = 0; i < m; i++)
        {
            delete[] A[i];
        }
        delete[] A;
    }
}

/////////////////////////////////////////////
// Wypelnia macierz zerami

void KMacierz::Zeros()
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            A[i][j] = 0;
    }
}
