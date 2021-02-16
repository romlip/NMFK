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

	inline unsigned PobierzNumer() const { return numer; };
	inline double PobierzX() const { return x; };
	inline double PobierzTemperature() const { return temperatura; };
};



