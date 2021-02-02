#include "pch.h"

#include "KWezel1D.h"

KWezel1D::KWezel1D() {
	numer = -1;
	x = -1;
	temperatura = -1;
}
KWezel1D::KWezel1D(unsigned inumer, float ix) {
	numer = inumer;
	x = ix;
	temperatura = -1;
}
void KWezel1D::UstawNumer(unsigned inumer)
{
	numer = inumer;
}
void KWezel1D::UstawTemperature(float itemp) {
	temperatura = itemp;
}

unsigned KWezel1D::PobierzNumer()
{
	return numer;
}
float KWezel1D::PobierzTemperature() {
	return temperatura;
}
float KWezel1D::PobierzX() {
	return x;
}