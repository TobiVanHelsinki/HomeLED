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
	int HorizontalOffset = 3;
	int VerticalOffset = 20;
	float Scaling = 0.45;
	void BuildTable(bool Verbose);
	int j = 0;
	inline int positive_modulo(int i, int n);

	int SinTabelSize = 256;
	float* SinTable;
};
