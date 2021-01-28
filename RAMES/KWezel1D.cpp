#include "pch.h"

#include "KWezel1D.h"
#include "KWezel1D.h"

KWezel1D::KWezel1D() {
	numer = -1;
	x = -1;
	temperatura = -1;
}
KWezel1D::KWezel1D(int inumer, float ix) {
	numer = inumer;
	x = ix;
	temperatura = -1;
}
void KWezel1D::UstawTemperature(float itemp) {
	temperatura = itemp;
}

int KWezel1D::PobierzNumer()
{
	return numer;
}
float KWezel1D::PobierzTemperature() {
	return temperatura;
}
float KWezel1D::PobierzX() {
	return x;
}