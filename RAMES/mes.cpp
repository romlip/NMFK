#include "pch.h"
#include "mes.h"
#include <iostream>

////////////////////////////////////////////////////////////
// Calka numeryczna z podanej funkcji f na przedziale (-1, 1)
// dla n okreslonych punktow przedzialu. Aktualnie n = 2, 3.

using namespace std;

double mes::calka_11(double f(double), int n)
{
    if (n > H.size()) {
        std::cerr << "Blont! za duze n\n";
        return 0;
    }

    double suma = 0;
    for (int i = 0; i < n; i++)
        suma += H[n][i] * f(ksi[n][i]);

    return suma;
}

double mes::calka_11(double f_ksi[], int n)
{
    double suma;
    for (int i = 0; i < n; i++)
        suma += H[n][i] * f_ksi[i];
    return suma;
}

