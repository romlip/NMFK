#include "pch.h"

#include "KObliczenia.h"
#include "mes.h"


KMacierz* KObliczenia::StworzLokalnaMacierzSztywnosci(KElement1D* element)
{
	KMacierz* k = new KMacierz(n);

	switch (n)
	{
	case 2:
		float kk[2][2] = {{ 1,-1 }, { -1,1 }};
		*k = (float**)kk; // k = [1, -1; -1, 1]
		break;
	}
	(*k) *= element->Pobierzk() / element->Pobierzh(); // lambda/h * [1,-1; -1,1]

	return k;
}

KWektor* KObliczenia::StworzLokalnyWektorNaprezen(KElement1D* element)
{
	KWektor* p = new KWektor(n);

	switch (n)
	{
	case 2:
		float pp[2] = { 1,1 };
		*p = (float*)pp; // p = [1,1]
		break;
	}
	*p *= element->Pobierzf() * element->Pobierzh() / 2; // p = f*h/2*[1,1]


	return p;
}

void KObliczenia::WypelnijK(KElement1D *e)
{
	KMacierz* k_e = StworzLokalnaMacierzSztywnosci(e);

	unsigned * nr_wezla = new unsigned[KElement1D::PobierzLiczbeWezlow()]; // tablica przechowujaca numery wezlow w elemencie

	for (short i = 1; i <= n; i++)
	{
		for (short j = 1; j <= n; j++)
		{
			(*urMES->PobierzA())((*e->PobierzWezly())[i - 1]->PobierzNumer(), (*e->PobierzWezly())[j - 1]->PobierzNumer()) += (*k_e)(i, j);
		}
	}

	delete k_e;
}

void KObliczenia::WypelnijP(KElement1D* e)
{
	KWektor* p_e = StworzLokalnyWektorNaprezen(e);

	unsigned* nr_wezla = new unsigned[KElement1D::PobierzLiczbeWezlow()];

	for (short i = 1; i <= n; i++)
	{
		(*urMES->PobierzY())((*e->PobierzWezly())[i - 1]->PobierzNumer()) += (*p_e)(i);
	}
	delete p_e;
}

void KObliczenia::WypelnijKWarunkamiBrzegowymi()
{
}

KObliczenia::KObliczenia()
{
	n = KElement1D::PobierzLiczbeWezlow();
	dane = nullptr;
	urMES = nullptr;
}

KObliczenia::~KObliczenia()
{
	if (urMES) delete urMES;
}

KWektor* KObliczenia::Licz(KDane* idane)
{
	dane = idane;
	n = KElement1D::PobierzLiczbeWezlow();
	unsigned rozmiar = (unsigned)dane->PobierzSiatke()->pWezly.size(); // ilosc wezlow w siatce
	KMacierz* K = new KMacierz(rozmiar); // globalna macierz sztywnosci, zainicjowana zerami
	KWektor* P = new KWektor(rozmiar); // globalny wektor naprezen, zainicjowany zerami

	for (auto it_e =  dane->PobierzSiatke()->elementy.begin(); it_e != dane->PobierzSiatke()->elementy.end(); ++it_e) // iteruj po wszystkich elementach
	{
		//KMacierz k(wymiar);
		//for (unsigned i = 1; i <= wymiar; i++)
		//{
		//	for (unsigned j = 1; j <= wymiar; j++)
		//	{
		//		double* f_ksi = new double[wymiar];
		//		for (int ii = 0; ii < n; ii++)
		//		{
		//			f_ksi[ii] = k_e * psiprim[i - 1](mes::ksi[n][ii]) * psiprim[j - 1](mes::ksi[n][ii] + b_e * psi[i - 1](mes::ksi[n][ii] * psi[j - 1](mes::ksi[n][ii])));
		//		}
		//		k(i, j) = 0.5 * h_e * mes::calka_11(f_ksi, n); // 
		//		delete f_ksi;
		//	}
		//}
		WypelnijK(&*it_e);
		WypelnijP(&*it_e);
	}
	WypelnijKWarunkamiBrzegowymi();

	return nullptr;
}

KMacierz* KObliczenia::PobierzGlobalnaMacierzSztywnosci()
{
	return urMES->PobierzA();
}
