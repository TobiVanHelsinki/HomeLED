#include "PulseMode.h"

PulseMode::PulseMode(ILEDProvider* leds) : SinMode(leds) 
{
}

void PulseMode::NextState()
{
	float scale = SinTable[positive_modulo(timepos, SinTabelSize)];
	auto color = Adafruit_NeoPixel::Color(
		(int)(CurrentColor_r * scale),
		(int)(CurrentColor_g * scale),
		(int)(CurrentColor_b * scale)
	);
	leds->fill(color, 0, leds->numPixels());
	timepos = positive_modulo(timepos + 1, SinTabelSize);
}

String PulseMode::ID = "pulse";

String PulseMode::GetID()
{
	return ID;
}

String PulseMode::Set(String Name, String Value)
{
	return SinMode::Set(Name, Value);
}