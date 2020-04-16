#pragma once
#include "SinMode.h"
class PulseMode :
	public SinMode
{
public:
	PulseMode(ILEDProvider* leds);

	static String ID;
	String GetID();
	String Set(String Name, String Value) override;
private:
};