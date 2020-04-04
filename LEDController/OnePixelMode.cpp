#include "OnePixelMode.h"

OnePixelMode::OnePixelMode(Adafruit_NeoPixel* leds) : ColorMode(leds)
{
	Serial.println("pixel");
}

String OnePixelMode::ID()
{
	return "pixel";
}

void OnePixelMode::NextState()
{
	leds->clear();
	leds->setPixelColor(j, CurrentColor);
	j = (j + 1) % leds->numPixels();
}