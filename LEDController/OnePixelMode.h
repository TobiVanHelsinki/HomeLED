#pragma once
#include "ColorMode.h"

class OnePixelMode :
	public ColorMode
{
public:
	OnePixelMode(ILEDProvider* leds);
	static String ID;
	String GetID();
	void NextState();
private:
	int j = 0;
};
