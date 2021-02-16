#pragma once

#include <vector>

#include "KWezel1D.h"

class KElement1D
{
private:
	static short int n; // ilosc wezlow w elemencie
	unsigned numer;
	double k; // wspolczynnik przewodnosci
	double f; // rozciagle zrodlo pola
	std::vector<KWezel1D*> vpWezly;

public:
	KElement1D();
	KElement1D(unsigned inumer, KWezel1D* lewy, KWezel1D* prawy, double ik, double iff);
	~KElement1D();

	static void UstawLiczbeWezlow(short int in);

	void UstawNumer(unsigned inumer);
	void UstawWezel(int i, KWezel1D* wezel);
	inline static short int PobierzLiczbeWezlow() { return n; };
	std::vector<KWezel1D*>* PobierzWezly() { return &vpWezly; };
	KWezel1D* PobierzWezel(int ity_wezel) const { return vpWezly[ity_wezel - 1]; };
	inline unsigned PobierzNumer() const { return n; };
	inline double Pobierzh() const { return (*(vpWezly.end() - 1))->PobierzX() - (*(vpWezly.begin()))->PobierzX(); };
	inline double Pobierzk() const { return k; };
	inline double Pobierzf() const { return f; };
	//inline double PobierzT() const { return 0; };
};

