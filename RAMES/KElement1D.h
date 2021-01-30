#pragma once

#include <vector>

#include "KWezel1D.h"

class KElement1D
{
private:
	static short int n; // ilosc wezlow w elemencie
	unsigned numer;
	float k; // wspolczynnik przewodnosci
	float f; // rozciagle zrodlo pola
	std::vector<KWezel1D*> wezly_e;

public:
	KElement1D();
	KElement1D(unsigned inumer, KWezel1D* lewy, KWezel1D* prawy, float ik, float iff);
	~KElement1D();

	static void UstawLiczbeWezlow(short int in);
	static short int PobierzLiczbeWezlow();

	std::vector<KWezel1D*>* PobierzWezly();
	int PobierzNumer();
	float Pobierzh();
	float Pobierzk();
	float Pobierzf();
	float PobierzT();
};

