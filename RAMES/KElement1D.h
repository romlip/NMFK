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
	static short int PobierzLiczbeWezlow();


	std::vector<KWezel1D*>* PobierzWezly();
	KWezel1D* PobierzWezel(int ity_wezel);
	unsigned PobierzNumer();
	double Pobierzh();
	double Pobierzk();
	double Pobierzf();
	double PobierzT();
};

