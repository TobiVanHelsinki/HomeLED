#pragma once
#include "ColorMode.h"

class OnePixelMode :
	public ColorMode
{
public:
	OnePixelMode(ILEDProvider* leds);
	String GetID();
	void NextState();
private:
	int j = 0;
};
