#include "TykeMode.h"
TykeMode::TykeMode(ILEDProvider* leds) : TwoColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(255, 0, 0);
	SecondColor = Adafruit_NeoPixel::Color(0, 0, 255);
}

void TykeMode::NextState()
{
	leds->setPixelColor(Position, CurrentColor);
	leds->setPixelColor(leds->numPixels() - Position, SecondColor);
	if (Position > leds->numPixels())
	{
		Position = 0;
	}
	else
	{
		Position++;
	}
}

String TykeMode::ID = "tyke";

String TykeMode::GetID()
{
	return ID;
}