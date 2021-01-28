#pragma once

#include <vector>
#include "KWezel1D.h"

class KElement1D
{
private:
	unsigned numer;
	float k; // wspolczynnik przewodnosci
	KWezel1D* lewy;
	KWezel1D* prawy;

public:
	KElement1D(unsigned inumer);
	KElement1D(unsigned inumer, KWezel1D* lewy, KWezel1D* prawy, float ik);
	~KElement1D();

	void UstawLewy(KWezel1D* ilewy);
	void UstawPrawy(KWezel1D* iprawy);

	KWezel1D* PobierzLewy();
	KWezel1D* PobierzPrawy();

	int PobierzNumer();
	float PobierzTemperature();
};

