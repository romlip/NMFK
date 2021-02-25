#include "pch.h"
#include "KMacierzSPF.h"
#include <cmath>

using namespace std;

// Kwadratowa symetryczna macierz pasmowa

KMacierzSPF::KMacierzSPF() : KMacierz(), mPasmo(0), mZero(0)
{
}

KMacierzSPF::KMacierzSPF(unsigned rozmiar, unsigned iPasmo)
{
    m = rozmiar;
    n = rozmiar;
    mPasmo = (iPasmo + 1) / 2;
    mZero = 0;
    Stworz();
}

KMacierzSPF::KMacierzSPF(const KMacierzSPF& copy)
{
    m = copy.m;
    n = copy.n;
    mPasmo = copy.mPasmo;
    mZero = 0;
    Stworz();
    for (unsigned i(0); i < mRozmiar; ++i)
            M[i] = copy.M[i];
}

void KMacierzSPF::Stworz()
{
    mRozmiar = m * mPasmo;
    M = new double[mRozmiar]{ 0 };
}
//
//double KMacierzSPF::DajIJ(unsigned i, unsigned j) const
//{
//    return (*this)(i, j);
//}

double& KMacierzSPF::operator()(unsigned i, unsigned j)
{
    if (i > m || j > n || i < 1 || j < 1)
        throw runtime_error("Indeks poza zakresem");

    if (abs((long)i - (long)j) <= mPasmo - 1)
    {
        if (i <= j)
            return M[(i - 1) * mPasmo + j - i];
        else
            return M[(j - 1) * mPasmo + i - j];
    }
    else
    {
        mZero = 0;
        return mZero;
    }
}

double KMacierzSPF::operator()(unsigned i, unsigned j) const
{
    if (i > m || j > n || i < 1 || j < 1)
        throw runtime_error("Indeks poza zakresem");

    if (abs((long)i - (long)j) <= mPasmo - 1)
    {
        if (i <= j)
            return M[(i - 1) * mPasmo + j - i];
        else
            return M[(j - 1) * mPasmo + i - j];
    }
    else
        return 0;
}

//////////////////////////////////////////////////////////////
// Skopiowanie tablicy do macierzy

KMacierzSPF& KMacierzSPF::operator=(const KMacierzSPF& iM)
{
    if (this == &iM)
        return *this;
    if (M) delete[] M;

    m = iM.m;
    n = iM.n;
    mPasmo = iM.mPasmo;
    Stworz();

    for (unsigned i{ 0 }; i < mRozmiar; ++i)
        M[i] = iM.M[i];

    return *this;
}

///////////////////////////////////////////////////////////
// Mnozenie macierzy przez macierz

//KMacierzSPF KMacierzSPF::operator*(const KMacierzSPF& iM)
//{
//    if (n != iM.m)
//        throw runtime_error("zly rozmiar macierzy");
//
//    KMacierzSPF pro(m, iM.n);
//    double buff = 0.0;
//    for (unsigned i{ 1 }; i <= m; ++i) {
//        for (unsigned j{ 1 }; j <= iM.n; ++j) {
//            buff = 0.0;
//            for (unsigned l{ 1 }; l <= n; ++l)
//                buff += (*this)(i, l) * iM.DajIJ(l, j);
//            pro(i, j) = buff;
//        }
//    }
//    return pro;
//}

///////////////////////////////////////////////////////////
// Mnozenie macierzy przez stala

//KMacierzSPF& KMacierzSPF::operator*=(double a)
//{
//    for (unsigned i = 0; i < m * mPasmo; i++)
//            this->M[i] *= a;
//
//    return *this;
//}

//KMacierzSPF& KMacierzSPF::operator+=(double a)
//{
//    for (unsigned i = 0; i < m * mPasmo; i++)
//            this->M[i] += a;
//
//    return *this;
//}
//
//bool KMacierzSPF::operator!=(double iskalar)
//{
//    for (unsigned i{ 0 }; i < m * mPasmo; ++i)
//            if (this->M[i] - iskalar == 0)
//                return true;
//
//    return false;
//}

KMacierzSPF KMacierzSPF::T() const
{
    KMacierzSPF trans(*this);

    return trans;
}

void KMacierzSPF::RozlozCholeskySelf()
{
    double suma;
    for (unsigned i(1); i <= m; ++i){
        for (unsigned j = 1; j <= i; ++j){
            suma = 0;
            if (i == j){
                for (unsigned k = (i - mPasmo+1 > 0 ? i - mPasmo+1 : 1); k <= j - 1; ++k)
                    suma += (*this)(i, k) * (*this)(i, k);
                (*this)(i, i) = sqrt((*this)(i, i) - suma);
            }
            else { // j < i
                for (unsigned k = (j - mPasmo + 1 > 0 ? j - mPasmo+1 : 1); k <= j - 1; ++k)
                    suma += (*this)(i, k) * (*this)(j, k);
                (*this)(i, j) = 1 / (*this)(j, j) * ((*this)(i, j) - suma);
            }
        }
    }
}

unsigned KMacierzSPF::PobierzPasmo() const
{
    return mPasmo * 2 -1;
}
