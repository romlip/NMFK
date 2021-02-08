#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "KWezel1D.h"
#include "KElement1D.h"

class KSiatka
{
private:
	int liczba_wezlow_w_elemencie = 2; //liczba wezlow w elemencie
	float mfXmin, mfXmax;
	unsigned rozmiar; // liczba wszystkich wezlow siatki
	std::vector<KWezel1D*> vpWezly; // wektor wskaznikow na wezly siatki
	std::vector<KElement1D> vElementy; // wektor elementow siatki
	std::vector<KWezel1D*> vpWezly_wczytane; // wektor wskaznikow na wezly wczytane z pliku z uwzglednieniem punktow zrodlowych
	std::vector<KElement1D> vStruktura; // wektor obszarow
	std::vector<KWezel1D*> vpWezly_zrodlowe;

public:
	friend class KPlik;
	friend class KObliczenia;

	KSiatka();
	~KSiatka();

	void UstawLiczbeWezlowWelemencie(int iliczba_wezlow);
	void UstawXmin(float iXmin);
	void UstawXmax(float iXmax);

	float PobierzXmin();
	float PobierzXmax();

	void NumerujWezly();

	KWezel1D* WezelIstnieje(float x_wezla);
	KElement1D* ZnajdzElementZpunktem(KWezel1D* punkt_zrodlowy);

	KWezel1D* DodajWezel(float x);
	void DodajElement(unsigned inr, float ixl, float ixp, float ik, float iif);
	//void WstawElement(unsigned inr, float ixl, float ixp, float k);
	void WstawElementZa(KWezel1D* iwezel);
	void DodajWezlyWewnetrzne();

	int PobierzLiczbeWezlowWelemencie();
	KWezel1D* PobierzWezel(unsigned nrwezla);
	std::vector<KWezel1D*>* PobierzWezly();
	std::vector<KWezel1D*>* PobierzWezlyWczytane();
	std::vector<KElement1D>* PobierzStrukture();
	std::vector<KElement1D>* PobierzElementy();
	//void WyswietlSiatke();
	//void WyswietlStrukture();

};

