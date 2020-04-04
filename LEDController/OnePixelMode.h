#pragma once
#include "ColorMode.h"

class OnePixelMode :
	public ColorMode
{
public:
	OnePixelMode(Adafruit_NeoPixel* leds);
	String ID();
	void NextState();
private:
	int j = 0;
};

