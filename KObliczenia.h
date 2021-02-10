
#pragma once
#include <vector>

#include "KDane.h"
#include "KMacierz.h"
#include "KWektor.h"
#include "KUkladRownan.h"
#include "KElement1D.h"
#include <iomanip>

enum EwarunkiI { REDUKCJA = 0, ANALITYCZNE2 = 1, PAYNE_IRONS = 2 };

class KObliczenia
{
private:
	EwarunkiI eWarunkiI;
	KDane* dane;
	KUkladRownan* urMES;

	KMacierz* StworzLokalnaMacierzSztywnosci(KElement1D* element);
	KWektorK* StworzLokalnyWektorNaprezen(KElement1D* element);

	void WypelnijK(KElement1D* e);
	void WypelnijP(KElement1D* e);

	void WypelnijUkladRownan(KElement1D* e);
	void WypelnijUkladRownanAnalitycznie(KElement1D* e);
	void WypelnijUkladRownanPayneIrons(KElement1D* e);

	void WypelnijKWarunkamiBrzegowymi();
	void WypelnijWarunkamiIRodzaju();
	void WypelnijWarunkamiIIRodzaju(KUkladRownan* ur);
	void WypelnijWarunkamiKonwekcyjnymi(KUkladRownan* ur);
	void WypelnijZrodlamiPunktowymi(KUkladRownan* ur);
	void WypelnijZrodlamiRozciaglymi();

	void UstawTemperatureWezlow();

	inline double ksi3(float X, float Xi, float Xj, float Xk) { return 2. * (X - Xj) / (Xk - Xi); }

	inline double N2i(float X, float Xi, float Xj) { return (Xj - X) / (Xj - Xi); };
	inline double N2j(float X, float Xi, float Xj) { return (X - Xi) / (Xj - Xi); };

	inline double N3i(float X, float Xi, float Xj, float Xk) { return -0.5 * ksi3(X, Xi, Xj, Xk) * (1. - ksi3(X, Xi, Xj, Xk)); };
	inline double N3j(float X, float Xi, float Xj, float Xk) { return 1. - ksi3(X, Xi, Xj, Xk) * ksi3(X, Xi, Xj, Xk); };
	inline double N3k(float X, float Xi, float Xj, float Xk) { return 0.5 * ksi3(X, Xi, Xj, Xk) * (1. + ksi3(X, Xi, Xj, Xk)); };

	inline float T2(float X, float Xi, float Ti, float Xj, float Tj) { return 	N2i(X, Xi, Xj) * Ti + N2j(X, Xi, Xj) * Tj; };
	inline float T3(float X, float Xi, float Ti, float Xj, float Tj, float Xk, float Tk) { return 	N3i(X, Xi, Xj, Xk) * Ti + N3j(X, Xi, Xj, Xk) * Tj + N3k(X, Xi, Xj, Xk) * Tk; };

public:
	bool mObliczeniaFlag;
	KObliczenia();
	~KObliczenia();

	void UstawUwzglednianieWarunkowI(int uwzglednianieWarunkowI);

	KDane* PobierzDane();
	KUkladRownan* PobierzUkladRownan();
	KMacierz* PobierzGlobalnaMacierzSztywnosci();
	KWektorK* PobierzGlobalnyWektorNaprezen();

	KWektorK* Licz(KDane* idane);
	void WypiszWynik(std::ostream& iplik, bool tylkoWezly = false);



};
