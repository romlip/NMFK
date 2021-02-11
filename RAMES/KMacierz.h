#pragma once

#include <iostream>

class KMacierz
{
protected:
	double* M; // nasza macierz
	unsigned m; //liczba wierszy
	unsigned n; // liczba kolumn
	virtual void Stworz();

public:
	KMacierz();
	KMacierz(const KMacierz &copy);
	KMacierz(int in);
	KMacierz(int im, int in);
	KMacierz(unsigned rozmiar, const double* tablica);
	KMacierz(unsigned im, unsigned in, const double* tablica);
	~KMacierz();

	virtual void Zeros();
	inline unsigned DajM();
	inline unsigned DajN();
	double DajIJ(unsigned i, unsigned j);
	virtual bool DodatnioOkreslona();
	virtual void Wypisz(std::ostream &out);

	double& operator()(unsigned i, unsigned j);

	virtual KMacierz& operator = (double** const tablica2D);

	KMacierz& operator=(const KMacierz &iM);
	KMacierz operator*(const KMacierz& iM);
	KMacierz operator+(const KMacierz& iM);
	KMacierz operator-(const KMacierz& iM);

	KMacierz operator*(double skalar);
	KMacierz& operator *= (double skalar);
	KMacierz& operator += (double skalar);

	//operator KWektorK() const;

	//////////////////////////////////////////////

	KMacierz KMacierz::getCofactor();
	double KMacierz::getDeterminant();
	KMacierz T();
	KMacierz Inverse();
	bool operator!=(double skalar);
};