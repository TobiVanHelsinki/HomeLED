#pragma once
#include <Adafruit_NeoPixel.h>

class ModeBase
{
public:
	ModeBase(Adafruit_NeoPixel* ledstouse);

	virtual String ID() = 0;
	virtual void NextState() = 0;

	virtual int NumberofParams();
	virtual String GetName(int Number);
	virtual String Get(String Name);
	virtual String Set(String Name, String Value);
	virtual void ICACHE_RAM_ATTR Interrupt(int Pin);

protected:
	Adafruit_NeoPixel* leds;
	uint32_t Wheel(byte WheelPos);
};
