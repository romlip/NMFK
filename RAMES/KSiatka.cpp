#include "pch.h"
#include "KSiatka.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>

using namespace std;


KSiatka::KSiatka() {
    Inicjalizuj();
}


KSiatka::~KSiatka() {
    WyczyscWezly(vpWezly);
    WyczyscWezly(vpWezly_wczytane);
}

KWezel1D* KSiatka::WezelIstnieje(double x_wezla, const vector<KWezel1D*> &vpW)
{
    for (auto it_w = vpW.begin(); it_w != vpW.end(); ++it_w)
        if ((*it_w)->PobierzX() == x_wezla) // jesli istnieje, pobierz wskaznik na niego
            return *it_w;

    return nullptr;
}

KElement1D* KSiatka::ZnajdzElementZpunktem(KWezel1D* punkt_zrodlowy)
{
    for (auto it_e = vElementy.begin(); it_e != vElementy.end(); ++it_e)
    {
        double xpz = punkt_zrodlowy->PobierzX();
        if (it_e->PobierzWezel(1)->PobierzX() < xpz && it_e->PobierzWezel(2)->PobierzX() > xpz)
            return &*it_e;
    }
    return nullptr;
}

void KSiatka::WstawElementZa(KWezel1D* iwezel)
{
    // znajdz element, w ktorym znajduje sie wezel
    for (auto it_e = vElementy.begin(); it_e != vElementy.end(); ++it_e)
    {
        double x_iwezla = iwezel->PobierzX();
        if (it_e->PobierzWezel(1)->PobierzX() < x_iwezla && it_e->PobierzWezel(2)->PobierzX() > x_iwezla)
        {
            KWezel1D* e_prawy = it_e->PobierzWezel(2);
            it_e->UstawWezel(2, iwezel);
            vElementy.insert(it_e + 1, KElement1D(it_e->PobierzNumer(), iwezel, e_prawy, it_e->Pobierzk(), it_e->Pobierzf()));
            break;
        }
    }
}

//////////////////////////////////////////////////
// Dodaje wezly wewnetrzne elementow zgodnie z iloscia podana 
// w liczba_wezlow_w_elemencie

void KSiatka::DodajWezlyWewnetrzne()
{
    if (liczba_wezlow_w_elemencie > 2)
    {
        double xl, h;
        KWezel1D* pNowyWezel;
        for (auto it_e = vElementy.begin(); it_e != vElementy.end(); ++it_e)
        {
            xl = it_e->PobierzWezel(1)->PobierzX();
            h = it_e->Pobierzh();
            for (unsigned i(0); i < liczba_wezlow_w_elemencie - 2; ++i)
            {
                pNowyWezel = DodajWezel(xl + (i + 1) * h / (liczba_wezlow_w_elemencie - 1));
                auto it_we = it_e->PobierzWezly()->end();
                it_e->PobierzWezly()->insert(it_we - 1, pNowyWezel);
            }
        }
    }
}

void KSiatka::ZaladujStrukturePierwotna()
{
    vElementy.clear();
    WyczyscWezly(vpWezly);

    for (auto ite = vStruktura.begin(); ite != vStruktura.end(); ++ite)
    {
        KElement1D e(*ite);
        KWezel1D* pW;
        unsigned nrLokalnyWezla = 1;
        for (auto itw = ite->PobierzWezly()->begin(); itw != ite->PobierzWezly()->end(); ++itw)
        {
            if (!(pW = WezelIstnieje((*itw)->PobierzX(), vpWezly)))
            {
                pW = new KWezel1D((**itw));
                vpWezly.push_back(pW);
            }
            e.UstawWezel(nrLokalnyWezla, pW);
            ++nrLokalnyWezla;
        }
        vElementy.push_back(e);
    }
}

void KSiatka::ZapamietajStrukturePierwotna()
{
    vStruktura.clear();
    WyczyscWezly(vpWezly_wczytane);

    for (auto ite = vElementy.begin(); ite != vElementy.end(); ++ite)
    {
        KElement1D e(*ite);
        KWezel1D* pW;
        unsigned nrLokalnyWezla = 1;
        for (auto itw = ite->PobierzWezly()->begin(); itw != ite->PobierzWezly()->end(); ++itw)
        {
            if (!(pW = WezelIstnieje((*itw)->PobierzX(), vpWezly_wczytane)))
            {
                pW = new KWezel1D((**itw));
                vpWezly_wczytane.push_back(pW);
            }
            e.UstawWezel(nrLokalnyWezla, pW);
            ++nrLokalnyWezla;
        }
        vStruktura.push_back(e);
    }
}

void KSiatka::ZagescWstepnie()
{
    if (mbZagescWstepnie)
    {
        bool bWstawiono;
        do {
            bWstawiono = false;
            for (auto it_e = vElementy.begin(); (it_e + 1) != vElementy.end(); ++it_e)
            {
                if (it_e->Pobierzh() > mfWspolczynnikZageszczania * (it_e + 1)->Pobierzh()){
                    KWezel1D* pNowyWezel = DodajWezel(it_e->PobierzWezel(1)->PobierzX() + it_e->Pobierzh() / 2.);
                    WstawElementZa(pNowyWezel);
                    bWstawiono = true;
                    break;
                }
                else if (mfWspolczynnikZageszczania * it_e->Pobierzh() < (it_e + 1)->Pobierzh()){
                    KWezel1D* pNowyWezel = DodajWezel((it_e + 1)->PobierzWezel(1)->PobierzX() + (it_e + 1)->Pobierzh() / 2.);
                    WstawElementZa(pNowyWezel);
                    bWstawiono = true;
                    break;
                }
            }
        } while (bWstawiono);
    }
}

void KSiatka::Finalizuj()
{
    ZaladujStrukturePierwotna();

    DodajPunktyZrodlowe();
    DodajWezlyWewnetrzne();// dodaje wezly wewnatrz elementu
    NumerujWezly();
}

void KSiatka::Zagesc()
{
    if (mbZagescXkrotnie)
    {
        for (unsigned i(0); i < muKrotnoscZageszczenia; ++i)
        {
            unsigned liczbaElementow = vElementy.size();
            for (unsigned j(0); j < liczbaElementow; ++j)
            {
                auto it_e = vElementy.begin() + 2 * j;
                KWezel1D* pNowyWezel = DodajWezel(it_e->PobierzWezel(1)->PobierzX() + it_e->Pobierzh() / 2.);
                WstawElementZa(pNowyWezel);
            }
        }
    }
}

void KSiatka::Inicjalizuj()
{
    mbWygenerowano = false;
    liczba_wezlow_w_elemencie = 2;
    mbZagescWstepnie = false;
    muKrotnoscZageszczenia = 0;
    mfXmin = 0 , mfXmax = 0;
}

void KSiatka::Wyczysc()
{
    WyczyscWezly(vpWezly); // delete wskazniki na wezly
    WyczyscWezly(vpWezly_wczytane);

    mvPunktyZrodlowe.clear();
    vElementy.clear();
    vStruktura.clear();

    Inicjalizuj();
}

void KSiatka::WyczyscWezly(std::vector<KWezel1D*> &vpW)
{
    if (!vpW.empty())
    {
        for (auto itpw = vpW.begin(); itpw != vpW.end(); ++itpw)
            if (*itpw) delete* itpw;
        vpW.clear();
    }
}

KWezel1D* KSiatka::DodajWezel(double x)
{
    KWezel1D* w = new KWezel1D((unsigned)vpWezly.size() + 1, x);
    vpWezly.push_back(w);
    return w;
}

//KSiatka::KSiatka(char*  nazwa_pliku) {
//    //ksiatka::wczytaj(nazwa_pliku);
//}


///////////////////////////////////////////////
// Dodaje element na koncu wektora elementow

void KSiatka::DodajElement(unsigned inr, double ixl, double ixp, double ik, double iif)
{
    KWezel1D *pxl = nullptr, *pxp = nullptr;

    //sprawdz czy wezel ixl juz istnieje
    if (!(pxl = WezelIstnieje(ixl, vpWezly_wczytane))) // jeslie nie, to dodaj go do wektora wezlow
    {
        pxl = new KWezel1D((unsigned)vpWezly_wczytane.size() + 1, ixl);
        vpWezly_wczytane.push_back(pxl);
    }

    //sprawdz czy wezel ixp juz istnieje
    if (!(pxp = WezelIstnieje(ixp, vpWezly_wczytane)))
    {
        pxp = new KWezel1D((unsigned)vpWezly_wczytane.size() + 1, ixp);
        vpWezly_wczytane.push_back(pxp);
    }

    vStruktura.emplace_back(KElement1D(inr, pxl, pxp, ik, iif)); //dodaj element na koncu
}

void KSiatka::DodajZrodloPunktowe(strukt_zrodlo_punktowe& zrodlo)
{
   mvPunktyZrodlowe.push_back(zrodlo);
}

void KSiatka::DodajPunktyZrodlowe()
{
    // stworz wektor wskaznikow na wezly bedace zrodlami punktowymi; sluzy do wypelnienia globalnego wektora naprezen
    for (auto it_zp = mvPunktyZrodlowe.begin(); it_zp != mvPunktyZrodlowe.end(); ++it_zp)
    {
        double x_zrodla = it_zp->x;
        KWezel1D* punkt_zrodlowy = WezelIstnieje(x_zrodla, vpWezly);
        if (punkt_zrodlowy == nullptr)
        {
            punkt_zrodlowy = DodajWezel(x_zrodla); // stworz nowy wezel w x
            WstawElementZa(punkt_zrodlowy);
        }
        it_zp->pz = punkt_zrodlowy;
    }
}

void KSiatka::Generuj()
{
    ZaladujStrukturePierwotna();
    DodajPunktyZrodlowe();

    ZagescWstepnie();
    Zagesc();
    DodajWezlyWewnetrzne();// dodaje wezly wewnatrz elementu
    NumerujWezly();
}

////////////////////////////////////////////////////////
// Do wczytanej z pliku struktury dodaje punkty zrodlowe
// i konieczne elementy

void KSiatka::UstawXmin(double iXmin)
{
    mfXmin = iXmin;
}

void KSiatka::UstawXmax(double iXmax)
{
    mfXmax = iXmax;
}

void KSiatka::UstawZageszczanieWstepne(bool czy)
{
    mbZagescWstepnie = czy;
}

void KSiatka::UstawKrotnoscZageszczenia(unsigned krotnosc)
{
    muKrotnoscZageszczenia = krotnosc;
}


void KSiatka::UstawCzyZagescXkrotnie(unsigned czyZagescicXkrotnie)
{
    mbZagescXkrotnie = czyZagescicXkrotnie;
}

void KSiatka::NumerujWezly()
{
    sort(vpWezly.begin(), vpWezly.end(), [](KWezel1D* const& a, KWezel1D* const& b) { return a->PobierzX() < b->PobierzX(); }); // posortuj wektor wezlow po x

    // ustaw numery wezlow w kolejnosci x
    int i = 1;
    for (auto it_w = vpWezly.begin(); it_w != vpWezly.end(); ++it_w)
    {
        (*it_w)->UstawNumer(i);
        i++;
    }
}

