#include "pch.h"
#include "KSiatka.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

KSiatka::KSiatka() {
    rozmiar = 0;
}

//KSiatka::KSiatka(char*  nazwa_pliku) {
//    //ksiatka::wczytaj(nazwa_pliku);
//}

void KSiatka::DodajElement(unsigned inr, float ixl, float ixp, float k)
{
    //dodawanie wezla z prawej
    if (pWezly.empty()) // jesli struktura jest pusta i wczytujemy pierwszy wezel ever
    {
        pWezly.emplace_back(new KWezel1D(1, ixl));
    }
    pWezly.emplace_back(new KWezel1D((*(pWezly.end() - 1))->PobierzNumer() + 1, ixp)); //wezel z prawej strony elementu

    //dodawanie elementu
    elementy.emplace_back(KElement1D(inr, *(pWezly.end() - 2), *(pWezly.end() - 1), k)); //dodajemy element na koncu
}

KWezel1D* KSiatka::PobierzWezel(unsigned nrwezla)
{
    return *(pWezly.begin() + nrwezla - 1);
}

//void KSiatka::WczytajTemperature(string nazwa_pliku) {
//    ifstream plik(nazwa_pliku);
//    if (plik.is_open()) {
//        int nrWezla;
//        float temperatura;
//        string line;
//
//        getline(plik, line);    //pomijamy pierwszy wiersz pliku
//        while (getline(plik, line)) {
//
//            istringstream iss(line);
//            iss >> nrWezla >> temperatura;
//            wezly[nrWezla - 1].UstawTemperature(temperatura);
//            nrWezla++;
//        }
//        plik.close();
//        cout << "Wczytano temperatury dla " << nrWezla - 1 << " wezlow." << endl << endl;
//    }
//    else
//        cout << "Nie udalo sie otworzyc pliku." << endl;
//}


//void KSiatka::WyswietlSiatke() {
//    cout << "Obecna KSiatka posiada " << wezly.size() << " wezly/ow:\n"
//        << "nr_wezla\twspolrzedna\ttemperatura" << endl;
//    for (auto it = wezly.begin(); it != wezly.end(); it++)
//        cout << it->PobierzNumer() << "\t\t" << fixed << setprecision(1) << it->PobierzX() << "\t\t" << it->PobierzTemperature() << endl;
//    cout << endl;
//}
//
//void KSiatka::WyswietlStrukture() {
//    cout << "Obecnie struktura posiada " << elementy.size() << " elementy/ow\n"
//        << "nr_elementu\ttemperatura" << endl;
//    for (auto it = elementy.begin(); it != elementy.end(); ++it)
//        cout << it->PobierzNumer() << "\t\t" << fixed << it->PobierzTemperature() << endl;
//    cout << endl;
//}

KSiatka::~KSiatka() {
    if (!pWezly.empty())
    {
        for (auto it_wezly = pWezly.begin(); it_wezly != pWezly.end(); ++it_wezly)
        {
            delete *(it_wezly);
        }
    }
}

