
#pragma once
#include <vector>

#include "KDane.h"
#include "KMacierz.h"
#include "KWektor.h"
#include "KUkladRownan.h"
#include "KElement1D.h"
#include <iomanip>

enum class EwarunkiI { REDUKCJA = 0, PAYNE_IRONS = 1 };

class KObliczenia
{

private:
	int mWyniki;
	unsigned mGestoscAproksymacji;
	double mfTmin, mfTmax;
	bool mbWynikiCzyZakres;
	unsigned mWynikiLiczbaPunktow;
	double mWynikiZakresMin, mWynikiZakresMax;
	double mWynikiStalyKrok;
	EwarunkiI eWarunkiI;
	KDane* pDane;
	KUkladRownan mUrMES;

	KMacierz* StworzLokalnaMacierzSztywnosci(KElement1D* element);
	KWektorK* StworzLokalnyWektorNaprezen(KElement1D* element);

	void WypelnijK(KElement1D* e);
	void WypelnijP(KElement1D* e);

	void WstawMacierzLokalna(KElement1D* e);
	void WstawMacierzLokalnaRedukcja(KElement1D* e);
	void WstawMacierzLokalnaPayneIrons(KElement1D* e);

	void WypelnijKWarunkamiBrzegowymi();
	void WypelnijWarunkamiIRodzaju();
	void WypelnijWarunkamiIIRodzaju();
	void WypelnijWarunkamiKonwekcyjnymi();
	void WypelnijZrodlamiPunktowymi();
	void WypelnijZrodlamiRozciaglymi();

	void UstawTemperatureWezlow();

	void UstawFormatWynikow(std::ostream& stream) const;

public:
	KObliczenia();
	~KObliczenia();

	void UstawUwzglednianieWarunkowI(int uwzglednianieWarunkowI);
	void UstawWynikiRozklad(int wyniki);
	void UstawWynikiGestoscAproksymacji(int gestosc);
	void UstawWynikiCzyZakres(bool calyZakres);
	void UstawWynikiZakres(double zakresMin, double zakresMax);
	void UstawWynikiLiczbaPunktow(unsigned liczbaPunktow);
	void UstawWynikiStalyKrok(double krok);

	KDane* PobierzDane();
	KUkladRownan* PobierzUkladRownan();
	KMacierz* PobierzGlobalnaMacierzSztywnosci();
	KWektorK* PobierzGlobalnyWektorNaprezen();

	void WyznaczTwZakresie(double xp, double xk, unsigned n, double* x) const;
	inline double PobierzTmin() const { return mfTmin; };
	inline double PobierzTmax() const { return mfTmax; };
	int Licz(KDane* idane);
	void WypiszWynik(std::ostream& iplik) const;

	void KObliczenia::WynikiAproksymuj(unsigned gestosc, std::ostream& iplik) const;
};
