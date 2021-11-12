#pragma once
#include "SinMode.h"
class PulseMode :
	public SinMode
{
public:
	PulseMode(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
	String Set(String Name, String Value) override;
private:
};