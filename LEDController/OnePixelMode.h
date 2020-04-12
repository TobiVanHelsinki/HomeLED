#pragma once
#include "ColorMode.h"

class OnePixelMode :
	public ColorMode
{
public:
	OnePixelMode(ILEDProvider* leds);
	String ID();
	void NextState();
private:
	int j = 0;
};
