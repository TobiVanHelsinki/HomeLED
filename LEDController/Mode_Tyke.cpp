#include "Mode_Tyke.h"
TykeMode::TykeMode(ILEDProvider* leds) : TwoColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(255, 0, 0);
	SecondColor = Adafruit_NeoPixel::Color(0, 0, 255);
}

void TykeMode::NextState()
{
	for (size_t s = 0; s < StepSize; s++)
	{
		leds->setPixelColor(Position + s, CurrentColor);
		leds->setPixelColor(leds->numPixels() - Position - s, SecondColor); 
	}
	if (Position > leds->numPixels())
	{
		Position = 0;
	}
	else
	{
		Position += StepSize + Skip; //TODO je nach startpos hat es komische effekte
	}
}

String TykeMode::ID = "tyke";

String TykeMode::GetID()
{
	return ID;
}