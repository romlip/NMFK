#pragma once

class KWezel1D
{
private:
	unsigned numer;
	double x; //wspolrzedna x wezla
	double temperatura; //K

public:
	KWezel1D();
	KWezel1D(unsigned inumer, double ix);

	void UstawX(double ix);
	void UstawNumer(unsigned inumer);
	void UstawTemperature(double);

	unsigned PobierzNumer();
	double PobierzX();
	double PobierzTemperature();
};



