#pragma once
#include "ColorMode.h"
class TykeMode :
	public ColorMode
{
public:
	TykeMode(ILEDProvider* leds);
	virtual void NextState();
	String ID();
	int NumberofParams() override;
	String GetName(int Number) override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
protected:

	int Position = 0;
	uint32_t ColorLeft = Adafruit_NeoPixel::Color(0, 0, 255);
	uint32_t ColorRight = Adafruit_NeoPixel::Color(0, 152, 20);
};
