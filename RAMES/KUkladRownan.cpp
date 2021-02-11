#include "pch.h"
#include "KUkladRownan.h"
#include <iostream>
#include <fstream>
using namespace std;

KUkladRownan::KUkladRownan()
{
	A = nullptr;
	B = nullptr;
	X = nullptr;
}

//KUkladRownan::KUkladRownan(unsigned ili_wezlow)
//{
//	A = new KMacierz(ili_wezlow);
//	B = new KWektorK(ili_wezlow);
//	X = new KWektorK(ili_wezlow);
//}

KUkladRownan::KUkladRownan(KMacierz* iA, KWektorK* iX, KWektorK* iB)
{
	A = iA;
	B = iB;
	X = iX;
}

KUkladRownan::~KUkladRownan()
{
	//if (A) delete A;
	//if (B) delete B;
	//if (X) delete X;
}

KMacierz* KUkladRownan::PobierzA()
{
	return A;
}

KWektorK* KUkladRownan::PobierzB()
{
	return B;
}

KWektorK* KUkladRownan::PobierzX()
{
	return X;
}


void KUkladRownan::UstawA(KMacierz* iA)
{
	A = iA;
}

void KUkladRownan::UstawB(KWektorK* iB)
{
	B = iB;
}

KWektorK* KUkladRownan::Rozwiaz(const char* imetoda)
{
	if (!A || !B)
	{
		cerr << "Blont! Nie ustawiono macierzy A i/lub wektora Y\n";
			return nullptr;
	}

	if (imetoda == "gradienty_sprzezone")
		return RozwiazGradientamiSprzezonymi();
	else if (imetoda == "nadrelaksacja")
		return RozwiazNadrelaksacja();
	else if (imetoda == "cholesky")
		return RozwiazCholesky();
	else
		return nullptr;
}

KWektorK* KUkladRownan::Rozwiaz(KMacierz* iA, KWektorK* iY, const char* imetoda)
{
	UstawA(iA);
	UstawB(iY);
	return Rozwiaz(imetoda);
}

KWektorK* KUkladRownan::RozwiazGradientamiSprzezonymi()
{
	// cialo liczace uklad gradyjentami spszenrzonymi

	return X;
}

KWektorK* KUkladRownan::RozwiazNadrelaksacja()
{
	if (X->DajM() != A->DajN()) {
		std::cout << "No *** nie.";
		return nullptr;
	}

	double Xmin = -5;
	double XmaX = 5;
	int nn = A->DajN();
	int mm = A->DajM();

	KWektorK x0(nn);
	for (unsigned i{ 0 }; i < x0.DajM(); ++i)
		x0(i + 1) = Xmin + (XmaX - Xmin) / nn * i;

	// omega i blad graniczny
	double w = 1.75;
	//double err = 1e-8;

/*
	# A = L + D + U
	# D(iagonal) - macierz diagonalna, której jedynymi elementami niezerowymi s¹ elementy macierzy A le¹ce na jej g³ównej przek¹tnej,
	# L(ower) - dolna macierz trójk¹tna zawieraj¹ca elementy macierzy A le¿¹ce wy³¹cznie pod jej g³ówn¹ przek¹tn¹,
	# U(pper) - górna macierz trójk¹tn¹ zawieraj¹ca elementy macierzy A le¿¹ce wy³¹cznie nad jej g³ówn¹ przek¹tn¹
*/
	KMacierz D(nn, nn);// = np.diag(np.diag(A))
	KMacierz L(nn, nn);// = np.tril(A, -1)
	KMacierz U(nn, nn);// = np.triu(A, 1)

	for (unsigned i{ 0 }; i < nn; ++i) {
		D(i + 1, i + 1) = (*A)(i + 1, i + 1);
		for (unsigned j{ 0 }; j < nn; ++j) {
			if (j < i)
				L(i + 1, j + 1) = (*A)(i + 1, j + 1);
			if (j > i)
				U(i + 1, j + 1) = (*A)(i + 1, j + 1);
		}
	}

	// # B = (wL + D) ^ –1((1–w)D – wU) oraz c = w(wL + D) ^ –1 b
	// G-S: B = -(L + D) ^ –1 U
	KMacierz BB((L * w + D).Inverse() * (D * (1 - w) - U * w));
	KWektorK c(((L * w + D).Inverse() * w) * (*B));

	// # X[k + 1] = B X[k] + c
	(*X) = f(BB, c, x0);
	KWektorK bul = ((*A) * (*X)) - (*B);
	while (bul != 0) {
		x0 = *X;
		(*X) = f(BB, c, x0);
		bul = (*A) * (*X) - (*B);
	}

	return X;
}

KWektorK* KUkladRownan::RozwiazCholesky()
{
	if (!A->DodatnioOkreslona()) // uzupelnic funkcje
		throw runtime_error("macierz nie jest dodatnio okreslona");

	unsigned m = A->DajM();
	//KMacierz L(m, m);

	// rozklad choleskyego-banachiewicza
	double suma;
	for (unsigned i(1); i <= m; ++i)
	{
		for (unsigned j(1); j <= i; ++j)
		{
			suma = 0;
			if (i == j)
			{
				for (unsigned k(1); k <= i - 1; ++k)
					suma += (*A)(i, k) * (*A)(i, k);
				(*A)(i, i) = sqrt((*A)(i, i) - suma);
			}
			else // j < i
			{
				for (unsigned k(1); k <= j - 1; ++k)
					suma += (*A)(i, k) * (*A)(j, k);
				(*A)(i, j) = 1 / (*A)(j, j) * ((*A)(i, j) - suma);
			}
		}
	}
	//KMacierz LT((*A).T()); // LT = L^T

	// Rozwiazujemy uklad L * Y = B
	KWektorK Y(m);
	KUkladRownan Ly_b(A, &Y, B);
	Ly_b.RozwiazEliminacja(0);

	(*A) = ((*A).T()); // LT = gorna trojkatna A
	// Rozwiazujemy LT * X = Y ;
	KUkladRownan LTx_y(A, X, &Y);
	LTx_y.RozwiazEliminacja(1);

	return X;
}

KWektorK* KUkladRownan::RozwiazEliminacja(int gorna_dolna)
{
	double suma;
	if (gorna_dolna == 0)
	{
		for (unsigned i(1); i <= A->DajM(); ++i)
		{
			suma = 0;
			for (unsigned j(1); j <= i - 1; ++j)
			{
				suma += (*X)(j) * (*A)(i, j);
			}
			(*X)(i) = ((*B)(i) - suma) / (*A)(i, i);
		}
	}
	else
	{
		for (unsigned i(A->DajM()); i >=1 ; --i)
		{
			suma = 0;
			for (unsigned j(A->DajM()); j >= i + 1; --j)
			{
				suma += (*X)(j) * (*A)(i, j);
			}
			(*X)(i) = ((*B)(i) - suma) / (*A)(i, i);
		}
	}
	return X;
}
//////////////////////////////
// A

KWektorK KUkladRownan::f(KMacierz& B, KWektorK& c, KWektorK& x0) {
	KWektorK temp(B * x0 + c);
	return temp;
}