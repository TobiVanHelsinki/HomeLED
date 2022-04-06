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
	int SinTabelSize = 256;
	float Multi = 1;
	float Scaling = 0.45;

	virtual void BuildTable(bool Verbose);
	int positive_modulo(int i, int n);
	float* SinTable;
	int timepos = 0;

};
