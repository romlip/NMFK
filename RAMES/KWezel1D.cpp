#include "pch.h"

#include "KWezel1D.h"

KWezel1D::KWezel1D() {
	numer = -1;
	x = -1;
	temperatura = -1;
}
KWezel1D::KWezel1D(unsigned inumer, double ix) {
	numer = inumer;
	x = ix;
	temperatura = -1;
}
void KWezel1D::UstawX(double ix)
{
	x = ix;
}
void KWezel1D::UstawNumer(unsigned inumer)
{
	numer = inumer;
}
void KWezel1D::UstawTemperature(double itemp) {
	temperatura = itemp;
}

unsigned KWezel1D::PobierzNumer()
{
	return numer;
}
double KWezel1D::PobierzTemperature() {
	return temperatura;
}
double KWezel1D::PobierzX() {
	return x;
}