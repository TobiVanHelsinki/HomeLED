#pragma once
#include "ModeBase.h"
#include "math.h"

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
	uint8_t CurrentColor_r = 229;
	uint8_t CurrentColor_g = 130;
	uint8_t CurrentColor_b = 0;
	uint16_t CurrentColor_h = 6200;
	uint8_t CurrentColor_s = 255;
	uint8_t CurrentColor_v = 229;
	void RefreshColorParts();
	void CalculateHSV(byte r, byte g, byte b);
};
