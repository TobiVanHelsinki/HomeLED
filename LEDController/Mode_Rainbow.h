#pragma once
#include "Mode_Sin.h"

constexpr auto id = "rainbow";
class RainbowMode :
	public SinMode
{
public:
	RainbowMode(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
private:
	bool DisturbingMode = false;
	int j = 0;
};
