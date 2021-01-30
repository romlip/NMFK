#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "KWezel1D.h"
#include "KElement1D.h"

class KSiatka
{
private:
	std::vector<KWezel1D*> pWezly; // wektor wskaznikow na wezly
	std::vector<KElement1D> elementy;
	unsigned rozmiar;

	KWezel1D* WezelIstnieje(float x_wezla);


public:
	friend class KPlik;
	friend class KObliczenia;

	KSiatka();
	~KSiatka();

	void DodajElement(unsigned inr, float ixl, float ixp, float ik, float iif);
	//void WstawElement(unsigned inr, float ixl, float ixp, float k);

	KWezel1D* PobierzWezel(unsigned nrwezla);
	//void WyswietlSiatke();
	//void WyswietlStrukture();

};

