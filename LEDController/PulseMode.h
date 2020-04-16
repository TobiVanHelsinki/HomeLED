#pragma once
#include "SinMode.h"
class PulseMode :
	public SinMode
{
public:
	PulseMode(ILEDProvider* leds);

	String GetID();
	String Set(String Name, String Value) override;
private:
};