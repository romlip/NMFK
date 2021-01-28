#pragma once
class KMacierz
{
protected:
	float** A; // nasza macierz
	unsigned m; //liczba wierszy
	unsigned n; // liczba kolumn
	virtual void Stworz();

public:
	KMacierz();
	KMacierz(int in);
	KMacierz(int im, int in);
	~KMacierz();

	void Zeros();

	float operator()(unsigned i, unsigned j);
};