#include "pch.h"

#include "KElement1D.h"
#include "KWezel1D.h"

KElement1D::KElement1D(unsigned inumer) {
	numer = inumer;
	lewy = NULL;
	prawy = NULL;
}

KElement1D::KElement1D(unsigned inumer, KWezel1D* ilewy, KWezel1D* iprawy, float ik) {
	numer = inumer;
	lewy = ilewy;
	prawy = iprawy;
	k = ik;
}
KElement1D::~KElement1D() {

}
void KElement1D::UstawLewy(KWezel1D* ilewy) {
	lewy = ilewy;
}
void KElement1D::UstawPrawy(KWezel1D* iprawy) {
	prawy = iprawy;
}

KWezel1D* KElement1D::PobierzLewy()
{
	return lewy;
}

KWezel1D* KElement1D::PobierzPrawy()
{
	return prawy;
}

int KElement1D::PobierzNumer()
{
	return numer;
}

float KElement1D::PobierzTemperature() {
	return (lewy->PobierzTemperature() + prawy->PobierzTemperature()) / 2;
}