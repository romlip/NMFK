#pragma once

class KWezel1D
{
private:
	unsigned numer;
	float x; //wspolrzedna x wezla
	float temperatura; //K

public:
	KWezel1D();
	KWezel1D(unsigned inumer, float ix);

	void UstawTemperature(float);

	unsigned PobierzNumer();
	float PobierzX();
	float PobierzTemperature();
};



