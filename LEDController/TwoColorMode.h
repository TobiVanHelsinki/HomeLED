#pragma once
#include "ColorMode.h"
class TwoColorMode :
	public ColorMode
{
protected:
	TwoColorMode(ILEDProvider* leds);

	std::vector<String> ParameterNames() override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;

	uint32_t SecondColor;
	//uint16_t SecondColor_h = 154;
	//uint8_t SecondColor_s = 255;
	//uint8_t SecondColor_v = 229;
	void RefreshSecondColor();
};
