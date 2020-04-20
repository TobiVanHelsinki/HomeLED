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

	uint32_t SecondColor = 0;
	uint8_t SecondColor_r = 0;
	uint8_t SecondColor_g = 0;
	uint8_t SecondColor_b = 0;
	uint16_t SecondColor_h = 0;
	uint8_t SecondColor_s = 0;
	uint8_t SecondColor_v = 0;
};
