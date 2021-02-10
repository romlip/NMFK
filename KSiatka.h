#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "KWezel1D.h"
#include "KElement1D.h"

struct strukt_zrodlo_punktowe {
	float x;
	float g;
	KWezel1D* pz;
};

class KSiatka
{
private:
	int liczba_wezlow_w_elemencie; //liczba wezlow w elemencie
	float mfWspolczynnikZageszczania;
	bool mbZagescWstepnie;
	unsigned muKrotnoscZageszczenia;
	float mfXmin, mfXmax;

	std::vector<strukt_zrodlo_punktowe> mvPunktyZrodlowe;

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
	void UstawZageszczanieWstepne(bool czy);
	void UstawKrotnoscZageszczenia(unsigned krotnosc);
	float PobierzXmin();
	float PobierzXmax();

	void NumerujWezly();

	KWezel1D* WezelIstnieje(float x_wezla);
	KElement1D* ZnajdzElementZpunktem(KWezel1D* punkt_zrodlowy);

	KWezel1D* DodajWezel(float x);
	void DodajElement(unsigned inr, float ixl, float ixp, float ik, float iif);
	void DodajZrodloPunktowe(strukt_zrodlo_punktowe& zrodlo);
	void DodajPunktyZrodlowe();
	//void WstawElement(unsigned inr, float ixl, float ixp, float k);
	void WstawElementZa(KWezel1D* iwezel);
	void DodajWezlyWewnetrzne();
	void Finalizuj();
	void ZagescWstepnie();
	void Zagesc();
	

	void Generuj();
	int PobierzLiczbeWezlowWelemencie();
	KWezel1D* PobierzWezel(unsigned nrwezla);
	std::vector<KWezel1D*>* PobierzWezly();
	std::vector<KWezel1D*>* PobierzWezlyWczytane();
	std::vector<KElement1D>* PobierzStrukture();
	std::vector<KElement1D>* PobierzElementy();
	std::vector<strukt_zrodlo_punktowe>* PobierzZrodlaPunktowe();

	//void WyswietlSiatke();
	//void WyswietlStrukture();

};

