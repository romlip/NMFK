
#pragma once
#include <vector>

#include "KDane.h"
#include "KMacierz.h"
#include "KWektor.h"
#include "KUkladRownan.h"
#include "KElement1D.h"
#include <iomanip>

class KObliczenia
{
private:
	KDane* dane;
	KUkladRownan* urMES;

	KMacierz* StworzLokalnaMacierzSztywnosci(KElement1D* element);
	KWektorK* StworzLokalnyWektorNaprezen(KElement1D* element);

	void WypelnijK(KElement1D* e);
	void WypelnijP(KElement1D* e);

	void WypelnijUkladRownan(KElement1D* e);

	void WypelnijKWarunkamiBrzegowymi();
	void WypelnijWarunkamiIRodzaju();
	void WypelnijWarunkamiIIRodzaju(KUkladRownan* ur);
	void WypelnijWarunkamiKonwekcyjnymi(KUkladRownan* ur);
	void WypelnijZrodlamiPunktowymi(KUkladRownan* ur);
	void WypelnijZrodlamiRozciaglymi();

	void UstawTemperatureWezlow();

public:
	bool mObliczeniaFlag;
	KObliczenia();
	~KObliczenia();

	KDane* PobierzDane();
	KUkladRownan* PobierzUkladRownan();
	KMacierz* PobierzGlobalnaMacierzSztywnosci();
	KWektorK* PobierzGlobalnyWektorNaprezen();

	KWektorK* Licz(KDane* idane);
	void WypiszWynik(std::ostream& iplik);
};
