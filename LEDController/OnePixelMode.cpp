#include "OnePixelMode.h"

OnePixelMode::OnePixelMode(ILEDProvider* leds) : ColorMode(leds)
{
	Serial.println("pixel");
}

String OnePixelMode::GetID()
{
	return "pixel";
}

void OnePixelMode::NextState()
{
	leds->clear();
	leds->setPixelColor(j, CurrentColor);
	j = (j + 1) % leds->numPixels();
}