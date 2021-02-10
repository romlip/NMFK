#pragma once

#include <vector>

namespace mes
{
	// wyklad 09, slajdy 35-36

	const std::vector<std::vector<double>> H = {
		{1.0, 1.0},
		{0.555555555555556, 0.888888888888889,0.555555555555556},
	};

	const std::vector<std::vector<double>> ksi = {
		{-0.577350269189626, 0.577350269189626},
		{-0.774596669241482, 0.0 ,0.774596669241482},
	};

	double calka_11(double f(double), int n);
	double calka_11(double f_x[], int n);

	namespace galerkin
	{
		namespace funkcje_ksztaltu
		{
			typedef double(*ppsi)(double);

			namespace liniowe
			{
				inline double psi1(double x) { return 0.5 * (1 - x); };
				inline double psi2(double x) { return 0.5 * (1 + x); };
				const ppsi psi[2] = { &psi1, &psi2 };

				inline double psi1prim(double x) { return -0.5; };
				inline double psi2prim(double x) { return 0.5; };
				const ppsi psiprim[2] = { &psi1prim, &psi2prim };
			}

			namespace kwadratowe
			{
				inline double psi1(double x) { return 0.5 * x * (x - 1); };
				inline double psi2(double x) { return 1 - x * x; };
				inline double psi3(double x) { return 0.5 * x * (x + 1); };
				const ppsi psi[3] = { &psi1, &psi2, &psi3 };

				inline double psi1prim(double x) { return x - 0.5; };
				inline double psi2prim(double x) { return -2 * x; };
				inline double psi3prim(double x) { return x + 0.5; };
				const ppsi psiprim[3] = { &psi1prim, &psi2prim, &psi3prim };
			}
		}
	}
}