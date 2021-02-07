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

KWezel1D* KSiatka::WezelIstnieje(float x_wezla)
{
    for (auto it_w = vpWezly.begin(); it_w != vpWezly.end(); ++it_w)
    {
        if ((*it_w)->PobierzX() == x_wezla) // jesli istnieje, pobierz wskaznik na niego
        {
            return *it_w;
        }
    }
    return nullptr;
}

KElement1D* KSiatka::ZnajdzElementZpunktem(KWezel1D* punkt_zrodlowy)
{
    for (auto it_e = vElementy.begin(); it_e != vElementy.end(); ++it_e)
    {
        float xpz = punkt_zrodlowy->PobierzX();
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
        float x_iwezla = iwezel->PobierzX();
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
        float xl, h;
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

KWezel1D* KSiatka::DodajWezel(float x)
{
    KWezel1D* w = new KWezel1D((unsigned)vpWezly.size() + 1, x);
    vpWezly.push_back(w);
    return w;
}

KSiatka::KSiatka() {

    rozmiar = 0;
}

//KSiatka::KSiatka(char*  nazwa_pliku) {
//    //ksiatka::wczytaj(nazwa_pliku);
//}


///////////////////////////////////////////////
// Dodaje element na koncu wektora elementow

void KSiatka::DodajElement(unsigned inr, float ixl, float ixp, float ik, float iif)
{
    KWezel1D *pxl = nullptr, *pxp = nullptr;

    //sprawdz czy wezel ixl juz istnieje
    if (!(pxl = WezelIstnieje(ixl))) // jeslie nie, to dodaj go do wektora wezlow
    {
        pxl = new KWezel1D((unsigned)vpWezly.size() + 1, ixl);
        vpWezly.push_back(pxl);
    }

    //sprawdz czy wezel ixp juz istnieje
    if (!(pxp = WezelIstnieje(ixp)))
    {
        pxp = new KWezel1D((unsigned)vpWezly.size() + 1, ixp);
        vpWezly.push_back(pxp);
    }

    vElementy.emplace_back(KElement1D(inr, pxl, pxp, ik, iif)); //dodaj element na koncu
}

int KSiatka::PobierzLiczbeWezlowWelemencie()
{
    return liczba_wezlow_w_elemencie;
}

KWezel1D* KSiatka::PobierzWezel(unsigned nrwezla)
{
    return *(vpWezly.begin() + nrwezla - 1);
}

std::vector<KWezel1D*>* KSiatka::PobierzWezly()
{
    return &vpWezly;
}

std::vector<KWezel1D*>* KSiatka::PobierzWezlyWczytane()
{
    return &vpWezly_wczytane;
}

std::vector<KElement1D>* KSiatka::PobierzStrukture()
{
    return &vStruktura;
}

std::vector<KElement1D>* KSiatka::PobierzElementy()
{
    return &vElementy;
}

KSiatka::~KSiatka() {
    if (!vpWezly.empty())
    {
        for (auto it_wezly = vpWezly.begin(); it_wezly != vpWezly.end(); ++it_wezly)
        {
            delete *(it_wezly);
        }
    }
    if (!vpWezly_wczytane.empty())
    {
        for (auto it_wezly = vpWezly.begin(); it_wezly != vpWezly.end(); ++it_wezly)
        {
            delete* (it_wezly);
        }
    }
}

////////////////////////////////////////////////////////
// Do wczytanej z pliku struktury dodaje punkty zrodlowe
// i konieczne elementy



void KSiatka::UstawLiczbeWezlowWelemencie(int iliczba_wezlow)
{
    liczba_wezlow_w_elemencie = iliczba_wezlow;
}

void KSiatka::NumerujWezly()
{
    sort(vpWezly.begin(), vpWezly.end(), [](KWezel1D* const& a, KWezel1D* const& b) { return a->PobierzX() < b->PobierzX(); }); // posortuj wektor wezlow po x

    // ustaw numery wezlow w kolejnosci x
    int i = 1;
    for (auto it_w : vpWezly)
    {
        it_w->UstawNumer(i);
        i++;
    }
}

