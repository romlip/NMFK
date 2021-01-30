#pragma once

#include <vector>

#include "KSiatka.h"

struct strukt_warunekI {
	float x;
	float T;
};

struct strukt_zrodlo_punktowe{
	float x;
	float f;
};

class KDane
{
private:
	std::vector<strukt_warunekI> warunkiI;
	std::vector<strukt_zrodlo_punktowe> zrodla_punktowe;
	KSiatka* siatka;

public:
	friend class KPlik;
	KDane();
	~KDane();

	void DodajWarunekI(strukt_warunekI warunekI);
	void DodajZrodloPunktowe(strukt_zrodlo_punktowe zrodlo);

	KSiatka* PobierzSiatke();

};

