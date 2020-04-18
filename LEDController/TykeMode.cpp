#include "TykeMode.h"
TykeMode::TykeMode(ILEDProvider* leds) : TwoColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(0, 0, 255);
	SecondColor = Adafruit_NeoPixel::Color(255, 0, 0);
}

void TykeMode::NextState()
{
	if (Position > leds->numPixels())
	{
		Position = 0;
	}
	else
	{
		Position++;
	}
	leds->setPixelColor(Position, CurrentColor);         //  Set pixel's color (in RAM)
	leds->setPixelColor(leds->numPixels() - Position, SecondColor);        //  Set pixel's color (in RAM)
}

String TykeMode::ID = "tyke";

String TykeMode::GetID()
{
	return ID;
}