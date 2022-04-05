#include "Mode_Pulse.h"

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
	for (int ledpos = 0; ledpos < leds->numPixels(); ledpos += StepSize + Skip)
	{
		leds->setPixelColor(ledpos, color);
	}
	timepos = positive_modulo(timepos + 1, SinTabelSize);
}



String PulseMode::ID = "pulse";

String PulseMode::GetID()
{
	return ID;
}

String PulseMode::HandleProperty(String Name, String Value)
{
	return SinMode::HandleProperty(Name, Value);
}