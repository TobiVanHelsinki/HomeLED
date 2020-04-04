#pragma once
#include "ColorMode.h"

class SinMode :
	public ColorMode
{
public:
	SinMode(Adafruit_NeoPixel* leds);
	virtual void NextState();
	String ID();
	int NumberofParams() override;
	String GetName(int Number) override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
protected:
	int HorizontalOffset = 2;
	int VerticalOffset = 5;
	float Scaling = 0.45;
	void BuildTable(bool Verbose);
	int j = 0;
	bool DebugOutput = false;
	inline int positive_modulo(int i, int n);

	int SinTabelSize = 256;
	float* SinTable;
};

