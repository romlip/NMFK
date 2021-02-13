#pragma once
#include "KMacierz.h"
class KMacierzSPF :
    public KMacierz
{
private:
    unsigned mPasmo;
    double mZero;

    void Stworz();

public:
    KMacierzSPF();
    KMacierzSPF(const KMacierzSPF& copy);
    KMacierzSPF(unsigned m, unsigned n);
    ~KMacierzSPF() { };
    //virtual double DajIJ(unsigned i, unsigned j) const;

    double& operator()(unsigned i, unsigned j);
    double operator()(unsigned i, unsigned j) const;

    KMacierzSPF& operator=(const KMacierzSPF& iM);
    KMacierzSPF T() const;
    //KMacierzSPF operator*(const KMacierzSPF& iM);
    //KMacierzSPF& operator*=(double a);
    //KMacierzSPF& operator+=(double a);

    //bool operator!=(double iskalar);
    void RozlozCholeskySelf();
    unsigned PobierzPasmo() const;
};

