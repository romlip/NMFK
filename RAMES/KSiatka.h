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
	int rozmiar;

public:
	friend class KPlik;

	KSiatka();
	//KSiatka(char* nazwa_pliku);
	~KSiatka();

	//void WczytajTemperature(std::string nazwa_pliku);
	void DodajElement(unsigned inr, float ixl, float ixp, float k);
	//void WstawElement(unsigned inr, float ixl, float ixp, float k);

	KWezel1D* PobierzWezel(unsigned nrwezla);
	//void WyswietlSiatke();
	//void WyswietlStrukture();

};

