#pragma once

#include <vector>

#include "KSiatka.h"

struct strukt_warunekI {
	double x;
	double T;
	KWezel1D* pw;
};

struct strukt_warunekII {
	double x;
	double q;
	KWezel1D* pWii;
};

struct strukt_warunek_konwekcyjny {
	double x;
	double h;
	double T_inf;
	KWezel1D* wk;
};

struct strukt_zrodlo_rozciagle{
	double xl;
	double xp;
	double g;
};

class KDane
{
private:
	double mfSkala;
	std::vector<strukt_warunekI> warunkiI;
	std::vector<strukt_warunekII> warunkiII;
	std::vector<strukt_warunek_konwekcyjny> warunki_konwekcyjne;

	std::vector<strukt_zrodlo_punktowe> zrodla_punktowe;
	std::vector<strukt_zrodlo_rozciagle> zrodla_rozciagle;

	KSiatka mSiatka;

public:
	bool mDaneFlag;
	friend class KPlik;
	KDane();
	~KDane();

	void UstawSkale(double iSkala);

	void ZastosujSkale();
	void OdstosujSkale();

	inline double PobierzSkale() { return mfSkala; };
	void FinalizujWczytywanie();
	void DodajWezlyWarI();
	void DodajWezlyWarII();

	void WyczyscDane();
	void DodajWezlyWarKon();
	void DodajWarunekI(strukt_warunekI& warunekI);
	void DodajWarunekII(strukt_warunekII& warunekII);
	void DodajWarunekKonwekcyjny(strukt_warunek_konwekcyjny& warunek_konwekcyjny);
	void DodajZrodloRozciagle(strukt_zrodlo_rozciagle& zrodlo_rozciagle);

	void SprawdzPoprawnoscDanych();

	std::vector<strukt_warunekI>* PobierzWarunkiI();
	std::vector<strukt_warunekII>* PobierzWarunkiII();
	std::vector<strukt_warunek_konwekcyjny>* PobierzWarunkiKonwekcyjne();
	std::vector<strukt_zrodlo_rozciagle>* PobierzZrodlaRozciagle();
	std::vector<strukt_warunekI>* PobierzWezlyWarI();
	KSiatka* PobierzSiatke();

	void Inicjalizuj();

	void Wyczysc();
};

