#pragma once

#include <iostream>

class KMacierz
{
protected:
	unsigned long mRozmiar; // Liczba elementow tablicy
	double* M; // nasza macierz
	unsigned m; //liczba wierszy
	unsigned n; // liczba kolumn
	unsigned mPasmo;
	virtual void Stworz();

public:
	KMacierz();
	KMacierz(const KMacierz &copy);
	KMacierz(int in);
	KMacierz(int im, int in);
	KMacierz(unsigned rozmiar, const double* tablica);
	KMacierz(unsigned im, unsigned in, const double* tablica);
	virtual ~KMacierz();

	virtual void Zeros();
	inline unsigned DajM() const {return m;};
	inline unsigned DajN() const { return n; };
	virtual double DajIJ(unsigned i, unsigned j) const;
	virtual bool DodatnioOkreslona() const;
	virtual void Wypisz(std::ostream &out) const;

	virtual double& operator()(unsigned i, unsigned j);

	virtual double operator()(unsigned i, unsigned j) const;

	virtual KMacierz& operator = (double** const tablica2D);

	virtual KMacierz& operator=(const KMacierz &iM);
	virtual KMacierz operator*(const KMacierz& iM) const;
	virtual KMacierz operator+(const KMacierz& iM) const;
	virtual KMacierz operator-(const KMacierz& iM) const;

	virtual KMacierz operator*(double skalar) const;
	virtual KMacierz& operator *= (double skalar);
	virtual KMacierz& operator += (double skalar);

	virtual bool operator!=(double skalar);
	//operator KWektorK() const;

	//////////////////////////////////////////////

	KMacierz KMacierz::getCofactor() const;
	double KMacierz::getDeterminant() const;
	KMacierz T() const;
	KMacierz Inverse() const;
	virtual void RozlozCholeskySelf();
	virtual unsigned PobierzPasmo() const;

};