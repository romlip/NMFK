#include "pch.h"
#include "KMacierz.h"
#include "KWektor.h"
#include <stdexcept>
#include <iomanip>
using namespace std;


/////////////////////////////////////////////////////////
// Domyslny konstruktor

KMacierz::KMacierz()
{
    m = 0;
    n = 0;
    mRozmiar = 0;
    M = nullptr;
}

KMacierz::KMacierz(const KMacierz & copy)
{
    m = copy.m;
    n = copy.n;
    Stworz();
    for (unsigned i(0); i < mRozmiar; ++i)
                M[i] = copy.M[i];
}

/////////////////////////////////////////////
// Tworzy macierz kwadratowa in x in i wypelnia ja zerami

KMacierz::KMacierz(int in): KMacierz(in, in)
{
}

/////////////////////////////////////////////
// Tworzy macierz im x in i wypelnia ja zerami

KMacierz::KMacierz(int im, int in): m(im), n(in)
{
    Stworz();
}

KMacierz::KMacierz(unsigned rozmiar, const double* tablica) : KMacierz(rozmiar, rozmiar)
{
    for (unsigned i(0); i < rozmiar * rozmiar; ++i)
            M[i] = *(tablica + static_cast<unsigned long long>(i));
}

KMacierz::KMacierz(unsigned im, unsigned in, const double* tablica): KMacierz(im, in)
{
    for (unsigned i(0); i < m * n; ++i)
            M[i] = *(tablica + static_cast<unsigned long long>(i));
}

////////////////////////////////////////////
// Domyslny destruktor;

KMacierz::~KMacierz()
{
    if (M) delete[] M;
}

void KMacierz::Stworz()
{
    mRozmiar = m * n;
    M = new double[mRozmiar]{ 0 };
}

////////////////////////////////////////////////////////////
// Zwraca element (i, j) macierzy. Indeksy zaczynaja sie od 1,
// nie od 0! Z metody powinno korzystaja wszystkie pozostale funkcje
// i powinna byc nadpisywana w klasie pochodnej

double& KMacierz::operator()(unsigned i, unsigned j)
{
    if (i > m || j > n || i < 1 || j < 1)
        throw runtime_error("Indeks poza zakresem");

    return this->M[(i - 1) * n + (j - 1)];
}

double KMacierz::operator()(unsigned i, unsigned j) const
{
    if (i > m || j > n || i < 1 || j < 1)
        throw runtime_error("Indeks poza zakresem");

    return this->M[(i - 1) * n + (j - 1)];
}

//////////////////////////////////////////////////////////////
// Skopiowanie tablicy do macierzy

KMacierz& KMacierz::operator=( double** const tablica2D)
{
    for (unsigned i = 0; i < m; i++)
        for (unsigned j = 0; j < n; j++)
            this->M[i * n + j] = tablica2D[i][j];

    return *this;
}

KMacierz& KMacierz::operator=(const KMacierz& iM)
{
    if (this == &iM)
        return *this;
    if (M) delete[] M;

    m = iM.m;
    n = iM.n;
    Stworz();

    for (unsigned i{ 1 }; i <= m; ++i)
        for (unsigned j{ 1 }; j <= n; ++j)
            (*this)(i, j) = iM(i, j);
 
    return *this;
}


///////////////////////////////////////////////////////////
// Mnozenie macierzy przez macierz

KMacierz KMacierz::operator*(const KMacierz& iM) const
{
    if (n != iM.m)
        throw runtime_error("zly rozmiar macierzy");

    KMacierz pro(m, iM.n);
    double buff = 0.0;
    for (unsigned i{ 1 }; i <= m; ++i) {
        for (unsigned j{ 1 }; j <= iM.n; ++j) {
            buff = 0.0;
            for (unsigned l{ 1 }; l <= n; ++l)
                buff += (*this)(i, l) * iM(l, j);
            pro(i, j) = buff;
        }
    }
    return pro;
}

KMacierz KMacierz::operator+(const KMacierz& iM) const
{
    if (this->m != iM.m || this->n != iM.n)
        throw runtime_error("zly rozmiar macierzy");

    KMacierz sum(m, n);
    for (unsigned i{ 1 }; i <= m; ++i)
        for (unsigned j{ 1 }; j <= n; ++j)
            sum(i, j) = (*this)(i, j) + iM(i, j);
    return sum;
}

KMacierz KMacierz::operator-(const KMacierz& iM) const
{
    if (this->m != iM.m || this->n != iM.n)
        throw runtime_error("zly rozmiar macierzy");

    KMacierz dif(m, n);
    for (unsigned i{ 1 }; i <= m; ++i)
        for (unsigned j{ 1 }; j <= n; ++j)
            dif(i, j) = (*this)(i, j) - iM(i, j);
    return dif;
}

KMacierz KMacierz::operator*(double iskalar) const
{
    KMacierz pro(m, n);
    for (unsigned i{ 1 }; i <= m; ++i)
        for (unsigned j{ 1 }; j <= n; ++j)
            pro(i, j) = (*this)(i, j) * iskalar;
    return pro;
}

///////////////////////////////////////////////////////////
// Mnozenie macierzy przez stala

KMacierz& KMacierz::operator*=(double a)
{
    for (unsigned i = 0; i < mRozmiar; i++)
            this->M[i] *= a;

    return *this;
}

KMacierz& KMacierz::operator+=(double a)
{
    for (unsigned i = 0; i < mRozmiar; i++)
        this->M[i] += a;

    return *this;
}

//KMacierz::operator KWektorK() const
//{
//    if (n != 1)
//        throw runtime_error("macierz ma wiecej niz jedna kolumne");
//
//}

bool KMacierz::operator!=(double iskalar)
{
    bool bul = false;
    for (unsigned i{ 0 }; i < mRozmiar; ++i)
        if (this->M[i] - iskalar == 0)
            return true;
            
    return false;
}


/////////////////////////////////////////////
// Wypelnia macierz zerami

void KMacierz::Zeros()
{
    for (unsigned i = 0; i < mRozmiar; i++)
        M[i] = 0;
}

double KMacierz::DajIJ(unsigned i, unsigned j) const
{
    if (i > m || j > n || i < 1 || j < 1)
        throw runtime_error("Zly indeks");

    return (*this)(i, j);
}

bool KMacierz::DodatnioOkreslona() const
{
    //for (unsigned i(1); i <= n; ++i)
    //{
    //    if ()
    //}
    return true;
}

void KMacierz::Wypisz(std::ostream &out) const
{
    for (unsigned i = 1; i <= m; i++) {
        for (unsigned j = 1; j <= n; j++)
            out << fixed << setprecision(4) << (*this)(i, j) << "\t";
        out << "\n";
    }
}

/////////////////////////////////////////////////
////////////////////////////////////////////////

KMacierz KMacierz::Inverse() const
{
    double det = getDeterminant();
    if (det == 0)
        throw std::runtime_error("Determinant is 0");

    double d = 1.0 / det;
    KMacierz inverted(m, n);
    KMacierz invertigation(m, m);

    for (size_t i{ 0 }; i < m; ++i)
        for (size_t j{ 0 }; j < m; ++j)
            invertigation(i + 1, j + 1) = M[i * n + j] * d;

    //return getTranspose(getCofactor(solution));
    invertigation = invertigation.getCofactor();

    for (size_t i{ 0 }; i < invertigation.DajM(); ++i)
        for (size_t j{ 0 }; j < invertigation.DajM(); ++j)
            inverted(i + 1, j + 1) = invertigation(i + 1, j + 1);

    return inverted.T();
}

void KMacierz::RozlozCholeskySelf()
{
    double suma;
    for (unsigned i(1); i <= m; ++i)
    {
        for (unsigned j(1); j <= i; ++j)
        {
            suma = 0;
            if (i == j)
            {
                for (unsigned k(1); k <= i - 1; ++k)
                    suma += (*this)(i, k) * (*this)(i, k);
                (*this)(i, i) = sqrt((*this)(i, i) - suma);
            }
            else // j < i
            {
                for (unsigned k(1); k <= j - 1; ++k)
                    suma += (*this)(i, k) * (*this)(j, k);
                (*this)(i, j) = 1 / (*this)(j, j) * ((*this)(i, j) - suma);
            }
        }
    }
}

unsigned KMacierz::PobierzPasmo() const
{
    return m;
}

double KMacierz::getDeterminant() const
{
    if (m != n)
        throw std::runtime_error("Matrix is not quadratic");
    unsigned dim = m;

    if (dim == 0) return 1;
    if (dim == 1) return M[0];
    if (dim == 2) return M[0] * M[3] - M[1] * M[2];

    double result = 0;
    int sign = 1;
    for (int i{ 0 }; i < dim; ++i) {
        //Submatrix
        KMacierz subMat(dim - 1, dim - 1);
        for (int mm = 1; mm < dim; mm++) {
            int z = 0;
            for (int nn = 0; nn < dim; nn++) {
                if (nn != i) {
                    subMat(mm , z + 1) = M[mm * n + nn];
                    z++;
                }
            }
        }
        //recursive call
        result += sign * M[i] * subMat.getDeterminant();
        sign = -sign;
    }

    return result;
}

KMacierz KMacierz::T() const
{
    KMacierz trans(n, m);

    for (unsigned i{ 1 }; i <= n; ++i)
        for (unsigned j{ 1 }; j <= m; ++j)
            trans(i, j) = (*this)(j, i);
    return trans;
}

KMacierz KMacierz::getCofactor() const
{
    if (m != n)
        throw std::runtime_error("Matrix is not quadratic");

    KMacierz solution(m, n);
    KMacierz subMat (m - 1, n - 1);
    for (unsigned i = 0; i < m; i++) {
        for (unsigned j = 0; j < n; j++) {
            int p = 0;
            for (unsigned x = 0; x < m; x++) {
                if (x == i) continue;
                int q = 0;

                for (unsigned y = 0; y < m; y++) {
                    if (y == j) continue;
                    subMat(p + 1, q + 1) = M[x * n + y];
                    q++;
                }
                p++;
            }
            solution(i + 1, j + 1) = pow(-1, i + j) * subMat.getDeterminant();
        }
    }
    return solution;
}