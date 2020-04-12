#pragma once
#include "ILEDProvider.h"
#include <Adafruit_NeoPixel.h>

class ModeBase
{
public:
	ModeBase(ILEDProvider* ledstouse);

	virtual String ID() = 0;
	virtual void NextState() = 0;

	virtual int NumberofParams();
	virtual String GetName(int Number);
	virtual String Get(String Name);
	virtual String Set(String Name, String Value);
	//virtual void ICACHE_RAM_ATTR Interrupt(int Pin);

protected:
	ILEDProvider* leds;
	uint32_t Wheel(byte WheelPos);
};
