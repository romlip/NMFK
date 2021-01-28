#pragma once

class KWezel1D
{
private:
	int numer;
	float x; //wspolrzedna x wezla
	float temperatura; //K

public:
	KWezel1D();
	KWezel1D(int inumer, float ix);

	void UstawTemperature(float);

	int PobierzNumer();
	float PobierzX();
	float PobierzTemperature();
};



