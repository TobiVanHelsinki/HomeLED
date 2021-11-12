#pragma once
#include "ColorMode.h"

class Mode_Demo :
	public ColorMode
{
public:
	Mode_Demo(ILEDProvider* leds);
	static String ID;
	String GetID();
	void NextState();
private:
	int j = 0;
};
