#pragma once

#include <vector>

#include "KSiatka.h"

struct strukt_warunekI {
	float x;
	float T;
	KWezel1D* pw;
};

struct strukt_warunekII {
	float x;
	float q;
	KWezel1D* pWii;
};

struct strukt_warunek_konwekcyjny {
	float x;
	float h;
	float T_inf;
	KWezel1D* wk;
};

struct strukt_zrodlo_punktowe{
	float x;
	float g;
	KWezel1D* pz;
};

struct strukt_zrodlo_rozciagle{
	float xl;
	float xp;
	float g;
};

class KDane
{
private:
	std::vector<strukt_warunekI> warunkiI;
	std::vector<strukt_warunekII> warunkiII;
	std::vector<strukt_warunek_konwekcyjny> warunki_konwekcyjne;

	std::vector<strukt_zrodlo_punktowe> zrodla_punktowe;
	std::vector<strukt_zrodlo_rozciagle> zrodla_rozciagle;

	KSiatka* siatka;

public:
	bool mDaneFlag;
	friend class KPlik;
	KDane();
	~KDane();

	void FinalizujWczytywanie();
	void DodajPunktyZrodlowe();
	void DodajWezlyWarI();
	void DodajWezlyWarII();

	void DodajWezlyWarKon();
	void DodajWarunekI(strukt_warunekI& warunekI);
	void DodajWarunekII(strukt_warunekII& warunekII);
	void DodajWarunekKonwekcyjny(strukt_warunek_konwekcyjny& warunek_konwekcyjny);
	void DodajZrodloPunktowe(strukt_zrodlo_punktowe& zrodlo);
	void DodajZrodloRozciagle(strukt_zrodlo_rozciagle& zrodlo_rozciagle);

	std::vector<strukt_warunekI>* PobierzWarunkiI();
	std::vector<strukt_warunekII>* PobierzWarunkiII();
	std::vector<strukt_warunek_konwekcyjny>* PobierzWarunkiKonwekcyjne();
	std::vector<strukt_zrodlo_punktowe>* PobierzZrodlaPunktowe() ;
	std::vector<strukt_zrodlo_rozciagle>* PobierzZrodlaRozciagle();
	std::vector<strukt_warunekI>* PobierzWezlyWarI();
	KSiatka* PobierzSiatke();

};

