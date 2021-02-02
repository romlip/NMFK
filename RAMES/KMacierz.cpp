#include "pch.h"
#include "KMacierz.h"
#include <stdexcept>

using namespace std;


/////////////////////////////////////////////////////////
// Domyslny konstruktor

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
}

/////////////////////////////////////////////
// Tworzy macierz im x in i wypelnia ja zerami

KMacierz::KMacierz(int im, int in)
{
    m = im;
    n = in;
    Stworz();
}

KMacierz::KMacierz(unsigned rozmiar, const float* tablica)
{
    m = rozmiar;
    n = rozmiar;
    Stworz();
    for (unsigned i(0); i < rozmiar; ++i)
    {
        for (unsigned j(0); j < rozmiar; ++j)
        {
            A[i * n + j] = *(tablica + i * n + j);
        }
    }
}

////////////////////////////////////////////
// Domyslny destruktor;

KMacierz::~KMacierz()
{
    if (A) delete A;
}
void KMacierz::Stworz()
{
    A = new float[static_cast<unsigned long long>(m) * n]();
}

////////////////////////////////////////////////////////////
// Zwraca element (i, j) macierzy. Indeksy zaczynaja sie od 1,
// nie od 0!

float& KMacierz::operator()(unsigned i, unsigned j)
{
    if (i > m || j > n || i < 1 || j < 1)
    {
        throw runtime_error("Indeks poza zakresem");
    }

    return this->A[(i - 1) * n + (j - 1)];
}

//////////////////////////////////////////////////////////////
// Skopiowanie tablicy do macierzy

KMacierz& KMacierz::operator=( float** const tablica2D)
{
    for (unsigned i = 0; i < m; i++)
    {
        for (unsigned j = 0; j < n; j++)
        {
            this->A[i * n + j] = tablica2D[i][j];
        }
    }
    return *this;
}


///////////////////////////////////////////////////////////
// Mnozenie macierzy przez stala a

KMacierz& KMacierz::operator*=(float a)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            this->A[i * n + j] *= a;
        }
    }
    return *this;
}

KMacierz& KMacierz::operator+=(float a)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            this->A[i * n + j] += a;
        }
    }
    return *this;
}




/////////////////////////////////////////////
// Wypelnia macierz zerami

void KMacierz::Zeros()
{
    for (int i = 0; i < m * n; i++)
    {
        A[i] = 0;
    }
}

unsigned KMacierz::DajM()
{
    return m;
}

unsigned KMacierz::DajN()
{
    return n;
}

float KMacierz::DajIJ(int i, int j)
{
    if (i > m || j > n)
        cerr << "Blont! Niepoprawny indeks.";
    return A[(i - 1) * n + (j - 1)];
}

void KMacierz::Wypisz(std::ostream &out)
{
    for (unsigned i= 0; i < m; i++)
    {
        for (unsigned j = 0; j < n; j++)
        {
            out << A[i * n + j] << "\t";
        }
        out << "\n";
    }
}
