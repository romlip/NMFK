#pragma once

#include <iostream>

class KMacierz
{
protected:
	float* M; // nasza macierz
	unsigned m; //liczba wierszy
	unsigned n; // liczba kolumn
	virtual void Stworz();

public:
	KMacierz();
	KMacierz(const KMacierz &copy);
	KMacierz(int in);
	KMacierz(int im, int in);
	KMacierz(unsigned rozmiar, const float* tablica);
	KMacierz(unsigned im, unsigned in, const float* tablica);
	~KMacierz();

	virtual void Zeros();
	inline unsigned DajM();
	inline unsigned DajN();
	float DajIJ(unsigned i, unsigned j);
	virtual bool DodatnioOkreslona();
	virtual void Wypisz(std::ostream &out);

	float& operator()(unsigned i, unsigned j);

	virtual KMacierz& operator = (float** const tablica2D);

	KMacierz& operator=(const KMacierz &iM);
	KMacierz operator*(const KMacierz& iM);
	KMacierz operator+(const KMacierz& iM);
	KMacierz operator-(const KMacierz& iM);

	KMacierz operator*(float skalar);
	KMacierz& operator *= (float skalar);
	KMacierz& operator += (float skalar);

	//operator KWektorK() const;

	//////////////////////////////////////////////

	KMacierz KMacierz::getCofactor();
	double KMacierz::getDeterminant();
	KMacierz T();
	KMacierz Inverse();
	bool operator!=(float skalar);
};