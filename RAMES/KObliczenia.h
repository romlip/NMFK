
#pragma once
#include <vector>

#include "KDane.h"
#include "KMacierz.h"
#include "KWektor.h"
#include "KUkladRownan.h"
#include "KElement1D.h"
#include <iomanip>

#define TYLKO_WEZLY FALSE
#define GESTOSC_APROKSYMACJI 6

enum EwarunkiI { REDUKCJA = 0, ANALITYCZNE2 = 1, PAYNE_IRONS = 2 };

class KObliczenia
{
private:
	int mWyniki;
	int mGestoscAproksymacji;

	EwarunkiI eWarunkiI;
	KDane* dane;
	KUkladRownan* urMES;

	KMacierz* StworzLokalnaMacierzSztywnosci(KElement1D* element);
	KWektorK* StworzLokalnyWektorNaprezen(KElement1D* element);

	void WypelnijK(KElement1D* e);
	void WypelnijP(KElement1D* e);

	void WypelnijUkladRownan(KElement1D* e);
	void WypelnijUkladRownanRedukcja(KElement1D* e);
	void WypelnijUkladRownanPayneIrons(KElement1D* e);

	void WypelnijKWarunkamiBrzegowymi();
	void WypelnijWarunkamiIRodzaju();
	void WypelnijWarunkamiIIRodzaju(KUkladRownan* ur);
	void WypelnijWarunkamiKonwekcyjnymi(KUkladRownan* ur);
	void WypelnijZrodlamiPunktowymi(KUkladRownan* ur);
	void WypelnijZrodlamiRozciaglymi();

	void UstawTemperatureWezlow();

	void UstawFormatWynikow(std::ostream& stream);

	inline double ksi3(double X, double Xi, double Xj, double Xk) { return 2. * (X - Xj) / (Xk - Xi); }

	inline double N2i(double X, double Xi, double Xj) { return (Xj - X) / (Xj - Xi); };
	inline double N2j(double X, double Xi, double Xj) { return (X - Xi) / (Xj - Xi); };

	inline double N3i(double X, double Xi, double Xj, double Xk) { return 0.5 * ksi3(X, Xi, Xj, Xk) * (ksi3(X, Xi, Xj, Xk) - 1); };
	inline double N3j(double X, double Xi, double Xj, double Xk) { return 1. - ksi3(X, Xi, Xj, Xk) * ksi3(X, Xi, Xj, Xk); };
	inline double N3k(double X, double Xi, double Xj, double Xk) { return 0.5 * ksi3(X, Xi, Xj, Xk) * (1. + ksi3(X, Xi, Xj, Xk)); };

	inline double T2(double X, double Xi, double Ti, double Xj, double Tj) { return 	N2i(X, Xi, Xj) * Ti + N2j(X, Xi, Xj) * Tj; };
	inline double T3(double X, double Xi, double Ti, double Xj, double Tj, double Xk, double Tk) { return 	N3i(X, Xi, Xj, Xk) * Ti + N3j(X, Xi, Xj, Xk) * Tj + N3k(X, Xi, Xj, Xk) * Tk; };

public:
	bool mObliczeniaFlag;
	KObliczenia();
	~KObliczenia();

	void UstawUwzglednianieWarunkowI(int uwzglednianieWarunkowI);
	void UstawWynikiTylkoWezly(int wyniki);
	void UstawWynikiGestoscAproksymacji(int gestosc);

	KDane* PobierzDane();
	KUkladRownan* PobierzUkladRownan();
	KMacierz* PobierzGlobalnaMacierzSztywnosci();
	KWektorK* PobierzGlobalnyWektorNaprezen();

	KWektorK* Licz(KDane* idane);
	void WypiszWynik(std::ostream& iplik, bool tylkoWezly = false);
};
