#include "pch.h"
#include "KDane.h"

KDane::KDane()
{
	siatka = new KSiatka();
}

KDane::~KDane()
{
	if (siatka) delete siatka;
}

void KDane::DodajWarunekI(strukt_warunekI warunekI)
{
	warunkiI.push_back(warunekI);
}

void KDane::DodajZrodloPunktowe(strukt_zrodlo_punktowe zrodlo)
{
	zrodla_punktowe.push_back(zrodlo);
}

KSiatka* KDane::PobierzSiatke()
{
	return siatka;
}
