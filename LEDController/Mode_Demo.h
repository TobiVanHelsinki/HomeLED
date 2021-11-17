#pragma once
#include "Mode_Color.h"

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
