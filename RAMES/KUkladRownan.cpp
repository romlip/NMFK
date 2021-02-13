#include "pch.h"
#include "KUkladRownan.h"
#include <iostream>
#include <fstream>

#include "KMacierzSPF.h"

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
	# D(iagonal) - macierz diagonalna, kt�rej jedynymi elementami niezerowymi s� elementy macierzy A le�ce na jej g��wnej przek�tnej,
	# L(ower) - dolna macierz tr�jk�tna zawieraj�ca elementy macierzy A le��ce wy��cznie pod jej g��wn� przek�tn�,
	# U(pper) - g�rna macierz tr�jk�tn� zawieraj�ca elementy macierzy A le��ce wy��cznie nad jej g��wn� przek�tn�
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

	// # B = (wL + D) ^ �1((1�w)D � wU) oraz c = w(wL + D) ^ �1 b
	// G-S: B = -(L + D) ^ �1 U
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

	A->RozlozCholeskySelf(); // A <- L + L^T

	unsigned m = A->DajM();
	KWektorK Y(m);

	// Rozwiazujemy uklad L * Y = B
	KUkladRownan Ly_b(A, &Y, B);
	Ly_b.RozwiazEliminacja(0);

	// Rozwiazujemy LT * X = Y ;
	KUkladRownan LTx_y(A, X, &Y);
	LTx_y.RozwiazEliminacja(1);

	return X;
}

KWektorK* KUkladRownan::RozwiazEliminacja(int gorna_dolna)
{
	double suma;
	unsigned promienPasma = (A->PobierzPasmo() + 1) / 2;
	if (gorna_dolna == 0)
	{
		for (unsigned i(1); i <= A->DajM(); ++i)
		{
			suma = 0;
			for (unsigned j = (i - promienPasma + 1 > 0 ? i - promienPasma + 1 : 1); j <= i - 1; ++j)
				suma += (*X)(j) * (*A)(i, j);
			(*X)(i) = ((*B)(i) - suma) / (*A)(i, i);
		}
	}
	else
	{
		for (unsigned i(A->DajM()); i >=1 ; --i)
		{
			suma = 0;
			for (unsigned j = (i + promienPasma -1 <= A->DajM() ? i + promienPasma - 1 : A->DajM()); j >= i + 1; --j)
				suma += (*X)(j) * (*A)(i, j);
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