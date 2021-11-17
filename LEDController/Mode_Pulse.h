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
	String HandleProperty(String Name, String Value) override;
private:
};