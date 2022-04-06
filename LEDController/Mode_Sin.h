#pragma once
#include "Mode_Color.h"

class SinMode :
	public ColorMode
{
public:
	SinMode(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
	std::vector<String> ParameterNames() override;
	String HandleProperty(String Name, String Value) override;
protected:
	//Width of a calculated curve, e.g. the width of a full sin curve TWO_PI or a complete hue round in rainbow
	int SinTabelSize = 256;
	//represents a number between 0, 0.01 and 1.20 but is stored as numbers between 0 and 120
	int Multi = 1;
	//Vertical scaling of the curve to avoid dark leds in sin mode
	float Scaling = 0.45;

	virtual void BuildTable(bool Verbose);
	int positive_modulo(int i, int n);
	float* SinTable;
	int timepos = 0;

};
