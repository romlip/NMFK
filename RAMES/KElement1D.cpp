#include "pch.h"

#include "KElement1D.h"

using namespace std;

short KElement1D::n = 2;

KElement1D::KElement1D() {
	k = 0;
	f = 0;
	numer = 0;
}

KElement1D::KElement1D(unsigned inumer, KWezel1D* ilewy, KWezel1D* iprawy, double ik, double iff) {
	numer = inumer;
	k = ik;
	f = iff;
	vpWezly.push_back(ilewy);
	vpWezly.push_back(iprawy);
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
	return &vpWezly;
}

void KElement1D::UstawWezel(int i, KWezel1D* wezel)
{
	vpWezly[i - 1] = wezel;
}

void KElement1D::UstawNumer(unsigned inumer)
{
	numer = inumer;
}

unsigned KElement1D::PobierzNumer()
{
	return numer;
}

KWezel1D* KElement1D::PobierzWezel(int ity_wezel)
{
	return vpWezly[ity_wezel - 1];
}

double KElement1D::Pobierzh()
{
	return (*(vpWezly.end() - 1))->PobierzX() - (*(vpWezly.begin()))->PobierzX();
}

double KElement1D::Pobierzk()
{
	return k;
}

double KElement1D::Pobierzf()
{
	return f;
}

double KElement1D::PobierzT() {
	return 0;
}