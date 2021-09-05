#pragma once
#include "ColorMode.h"

class SinMode :
	public ColorMode
{
public:
	SinMode(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
	std::vector<String> ParameterNames() override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
protected:
	int SinTabelSize = 256;
	int Multi = 1;
	float Scaling = 0.45;

	void BuildTable(bool Verbose);
	inline int positive_modulo(int i, int n);
	float* SinTable;
	int timepos = 0;

};
