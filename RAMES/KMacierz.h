#pragma once

#include <iostream>

class KMacierz
{
protected:
	float* A; // nasza macierz
	unsigned m; //liczba wierszy
	unsigned n; // liczba kolumn
	virtual void Stworz();

public:
	KMacierz();
	KMacierz(int in);
	KMacierz(int im, int in);
	~KMacierz();

	void Zeros();
	void Wypisz(std::ostream &out);

	float& operator()(unsigned i, unsigned j);
	virtual KMacierz& operator = (float** const tablica2D);

	KMacierz& operator *= (float a);
	KMacierz& operator += (float a);
};