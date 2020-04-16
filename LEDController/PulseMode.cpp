#include "PulseMode.h"

PulseMode::PulseMode(ILEDProvider* leds) : SinMode(leds)
{
	SinMode::Set("vo", "0");
}

String PulseMode::GetID()
{
	return "pulse";
}

String PulseMode::Set(String Name, String Value)
{
	if (Name == "vo")
	{
		return SinMode::Set("vo", "0");
	}
	else
	{
		return SinMode::Set(Name, Value);
	}
}