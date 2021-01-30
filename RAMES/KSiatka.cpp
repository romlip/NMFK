#include "pch.h"
#include "KSiatka.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

KWezel1D* KSiatka::WezelIstnieje(float x_wezla)
{
    for (auto it_w = pWezly.begin(); it_w != pWezly.end(); ++it_w)
    {
        if ((*it_w)->PobierzX() == x_wezla) // jesli istnieje, pobierz wskaznik na niego
        {
            return *it_w;
        }
    }
    return nullptr;
}

KSiatka::KSiatka() {
    rozmiar = 0;
}

//KSiatka::KSiatka(char*  nazwa_pliku) {
//    //ksiatka::wczytaj(nazwa_pliku);
//}

void KSiatka::DodajElement(unsigned inr, float ixl, float ixp, float ik, float iif)
{
    KWezel1D *pxl = nullptr, *pxp = nullptr;

    //sprawdz czy wezel ixl juz istnieje
    if (!(pxl = WezelIstnieje(ixl))) // jeslie nie, to dodaj go do wektora wezlow
    {
        if (pWezly.empty())
            pWezly.emplace_back(new KWezel1D(1, ixl)); // jesli wektor nie istnieje, perwszy wezel ma numer 1
        else
            pWezly.emplace_back(new KWezel1D((*(pWezly.end() - 1))->PobierzNumer() + 1, ixl));

        pxl = *(pWezly.end() - 1);
    }

    //sprawdz czy wezel ixp juz istnieje
    if (!(pxp = WezelIstnieje(ixp)))
    {
        if (pWezly.empty())
            pWezly.emplace_back(new KWezel1D(1, ixp));
        else
            pWezly.emplace_back(new KWezel1D((*(pWezly.end() - 1))->PobierzNumer() + 1, ixp));

        pxp = *(pWezly.end() - 1);
    }

    //dodawanie elementu
    elementy.emplace_back(KElement1D(inr, pxl, pxp, ik, iif)); //dodajemy element na koncu
}

KWezel1D* KSiatka::PobierzWezel(unsigned nrwezla)
{
    return *(pWezly.begin() + nrwezla - 1);
}

KSiatka::~KSiatka() {
    if (!pWezly.empty())
    {
        for (auto it_wezly = pWezly.begin(); it_wezly != pWezly.end(); ++it_wezly)
        {
            delete *(it_wezly);
        }
    }
}

