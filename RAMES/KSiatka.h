#pragma once

#include <vector>
#include <fstream>
#include <string>

#include "KWezel1D.h"
#include "KElement1D.h"

struct strukt_zrodlo_punktowe {
	double x;
	double g;
	KWezel1D* pz;
};

class KSiatka
{
private:
	bool mbWygenerowano;
	int liczba_wezlow_w_elemencie; //liczba wezlow w elemencie
	const double mfWspolczynnikZageszczania = 3;
	bool mbZagescWstepnie;
	unsigned muKrotnoscZageszczenia;
	double mfXmin, mfXmax;
	double mfTmin, mfTmax;

	std::vector<strukt_zrodlo_punktowe> mvPunktyZrodlowe;

	std::vector<KWezel1D*> vpWezly; // wektor wskaznikow na wezly siatki
	std::vector<KElement1D> vElementy; // wektor elementow siatki
	std::vector<KWezel1D*> vpWezly_wczytane; // wektor wskaznikow na wezly wczytane z pliku z uwzglednieniem punktow zrodlowych
	std::vector<KElement1D> vStruktura; // wektor obszarow

	void WstawElementZa(KWezel1D* iwezel);
	void DodajWezlyWewnetrzne();
	void ZaladujStrukturePierwotna();
	void ZapamietajStrukturePierwotna();
	void WyczyscWezly(std::vector<KWezel1D*>& vpW);

	void ZagescWstepnie();
	void Zagesc();
	void Inicjalizuj();

public:
	friend class KPlik;
	friend class KObliczenia;

	KSiatka();
	~KSiatka();

	inline void UstawLiczbeWezlowWelemencie(int iliczba_wezlow) { liczba_wezlow_w_elemencie = iliczba_wezlow; };
	void UstawXmin(double iXmin);
	void UstawXmax(double iXmax);
	void UstawZageszczanieWstepne(bool czy);
	void UstawKrotnoscZageszczenia(unsigned krotnosc);


	void NumerujWezly();
	KWezel1D* WezelIstnieje(double x_wezla, const std::vector<KWezel1D*>& vpW);
	KElement1D* ZnajdzElementZpunktem(KWezel1D* punkt_zrodlowy);
	KWezel1D* DodajWezel(double x);
	void DodajElement(unsigned inr, double ixl, double ixp, double ik, double iif);
	void DodajZrodloPunktowe(strukt_zrodlo_punktowe& zrodlo);
	void DodajPunktyZrodlowe();
	void Finalizuj();
	void Generuj();

	inline int PobierzLiczbeWezlowWelemencie() const { return liczba_wezlow_w_elemencie; };
	inline KWezel1D* PobierzWezel(unsigned nrwezla) { return *(vpWezly.begin() + nrwezla - 1); };
	inline double PobierzTmin() const { return mfTmin; };
	inline double PobierzTmax() const { return mfTmax; };
	inline double PobierzXmin() const { return mfXmin; };
	inline double PobierzXmax() const { return mfXmax; };
	inline std::vector<KWezel1D*>* PobierzWezly() { return &vpWezly; };
	inline std::vector<KWezel1D*>* PobierzWezlyWczytane() { return&vpWezly_wczytane; };
	inline std::vector<KElement1D>* PobierzStrukture() { return &vStruktura; };
	inline std::vector<KElement1D>* PobierzElementy() { return &vElementy; };
	inline std::vector<strukt_zrodlo_punktowe>* PobierzZrodlaPunktowe() { return &mvPunktyZrodlowe; };
	inline unsigned PobierzKrotnoscZageszczenia() { return muKrotnoscZageszczenia; };

	void Wyczysc();

};

