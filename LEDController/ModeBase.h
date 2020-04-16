#pragma once
#include "ILEDProvider.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class ModeBase
{
public:
	ModeBase(ILEDProvider* ledstouse);

	virtual String GetID() = 0;
	virtual void NextState() = 0;

	virtual std::vector<String> ParameterNames();
	virtual String Get(String Name);
	virtual String Set(String Name, String Value);

protected:
	ILEDProvider* leds;
	uint32_t Wheel(byte WheelPos);
};
