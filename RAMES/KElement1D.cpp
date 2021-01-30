#include "pch.h"

#include "KElement1D.h"

using namespace std;

short KElement1D::n = 2;

KElement1D::KElement1D() {
	k = 0;
	f = 0;
	numer = 0;
}

KElement1D::KElement1D(unsigned inumer, KWezel1D* ilewy, KWezel1D* iprawy, float ik, float iff) {
	numer = inumer;
	k = ik;
	f = iff;
	wezly_e.push_back(ilewy);
	wezly_e.push_back(iprawy);
}

KElement1D::~KElement1D() 
{

}

short int KElement1D::PobierzLiczbeWezlow()
{
	return n;
}

void KElement1D::UstawLiczbeWezlow(short int in)
{
	n = in;
}

std::vector<KWezel1D*>* KElement1D::PobierzWezly()
{
	return &wezly_e;
}

int KElement1D::PobierzNumer()
{
	return numer;
}

float KElement1D::Pobierzh()
{
	return (*(wezly_e.end() - 1))->PobierzX() - (*(wezly_e.begin()))->PobierzX();
}

float KElement1D::Pobierzk()
{
	return k;
}

float KElement1D::Pobierzf()
{
	return f;
}

float KElement1D::PobierzT() {
	return 0;
}