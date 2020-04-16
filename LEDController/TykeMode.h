#pragma once
#include "ColorMode.h"
class TykeMode :
	public ColorMode
{
public:
	TykeMode(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
	std::vector<String> ParameterNames() override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
protected:

	int Position = 0;
	uint32_t ColorLeft = Adafruit_NeoPixel::Color(0, 0, 255);
	uint32_t ColorRight = Adafruit_NeoPixel::Color(0, 152, 20);
};
