#pragma once
#include "ModeBase.h"

class ColorMode :
	public ModeBase
{
public:
	ColorMode(ILEDProvider* leds);
	static String ID;
	String GetID();
	void NextState();
	std::vector<String> ParameterNames() override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
protected:
	uint32_t CurrentColor = 0xE58200;
	uint32_t CurrentColor_r = 229;
	uint32_t CurrentColor_g = 130;
	uint32_t CurrentColor_b = 0;
	void RefreshColor();
};
