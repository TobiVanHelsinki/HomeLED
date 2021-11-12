#include "OnePixelMode.h"

OnePixelMode::OnePixelMode(ILEDProvider* leds) : ColorMode(leds) //TODO in einen LED Test / vorschau mode umwandeln
{
	SERIALWRITELINE("pixel");
	leds->clear();
}

String OnePixelMode::ID = "pixel";

String OnePixelMode::GetID()
{
	return ID;
}

void OnePixelMode::NextState()
{
	leds->setPixelColor(j, 0);
	j = (j + 1) % leds->numPixels(); //TODO Test
	leds->setPixelColor(j, CurrentColor);
}