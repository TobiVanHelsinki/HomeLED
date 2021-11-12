#include "PulseMode.h"

PulseMode::PulseMode(ILEDProvider* leds) : SinMode(leds) //TODO sinus rausnehmen, selbst neu machen
{
	SinMode::Set("vo", "0");
}

String PulseMode::ID = "pulse";

String PulseMode::GetID()
{
	return ID;
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