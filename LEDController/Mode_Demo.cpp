#include "Mode_Demo.h"

Mode_Demo::Mode_Demo(ILEDProvider* leds) : ColorMode(leds) 
{
	SERIALWRITELINE("demo");
	leds->clear();
}

String Mode_Demo::ID = "demo";

String Mode_Demo::GetID()
{
	return ID;
}

void Mode_Demo::NextState()
{
	leds->setPixelColor(j, 0);
	j = (j + 1) % leds->numPixels();
	leds->setPixelColor(j, CurrentColor);
}