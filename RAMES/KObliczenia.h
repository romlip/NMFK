
#pragma once
#include <vector>

#include "KDane.h"
#include "KMacierz.h"
#include "KWektor.h"
#include "KUkladRownan.h"
#include "KElement1D.h"

class KObliczenia
{
private:
	KDane* dane;
	KUkladRownan* urMES;

	KMacierz* StworzLokalnaMacierzSztywnosci(KElement1D* element);
	KWektor* StworzLokalnyWektorNaprezen(KElement1D* element);

	void WypelnijK(KElement1D* e);
	void WypelnijP(KElement1D* e);

	void WypelnijUkladRownan(KElement1D* e);

	void WypelnijKWarunkamiBrzegowymi();
	void WypelnijWarunkamiIRodzaju();
	void WypelnijWarunkamiIIRodzaju();
	void WypelnijWarunkamiKonwekcyjnymi(KUkladRownan* ur);
	void WypelnijZrodlamiPunktowymi(KUkladRownan* ur);
	void WypelnijZrodlamiRozciaglymi();

public:
	KObliczenia();
	~KObliczenia();

	KMacierz* PobierzGlobalnaMacierzSztywnosci();
	KWektor* PobierzGlobalnyWektorNaprezen();

	KWektor* Licz(KDane* idane);
};
