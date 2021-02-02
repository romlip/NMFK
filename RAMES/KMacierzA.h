#pragma once

/*#include <stdio.h>
#include <fstream> // for file access
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <cmath>*/

#include <vector>
#include <tuple>

class KMacierzA
{
protected:
	unsigned w;		// liczba wierszy
	unsigned k;		// liczba kolumn
	std::vector<std::vector<float>> M;

public:
	KMacierzA();
	KMacierzA(unsigned iw);
	KMacierzA(unsigned iw, unsigned ik);
	KMacierzA(const KMacierzA& iM);	//kopiujacy
	~KMacierzA();

	//przeciazenia operatorow
	KMacierzA operator+(KMacierzA& iM);
	KMacierzA operator-(KMacierzA& iM);
	KMacierzA operator*(KMacierzA& iM);
//	void operator=(KMacierzA& iM);
	bool KMacierzA::operator!=(float skalar);
	KMacierzA operator+(float iskalar);
	KMacierzA operator-(float iskalar);
	KMacierzA operator*(float iskalar);
	KMacierzA operator/(float iskalar);

	float& operator()(const unsigned&, const unsigned&);

	unsigned getWiersze() const;
	unsigned getKolumny() const;
	void Drukuj() const;

	KMacierzA T();
	KMacierzA Inverse();


	/*
	// Power Iteration
	std::tuple<KMacierz, float, int> powerIter(unsigned, float);
	// Deflation
	KMacierz deflation(KMacierz&, float&);
	*/
};