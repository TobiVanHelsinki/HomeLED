#include "LEDProvider_Analog.h"

LEDProvider_Analog::LEDProvider_Analog(uint8_t r, uint8_t g, uint8_t b)
{
	_PinR = r;
	_PinG = g;
	_PinB = b;
}

void LEDProvider_Analog::begin(void)
{
	pinMode(_PinR, OUTPUT);
	pinMode(_PinG, OUTPUT);
	pinMode(_PinB, OUTPUT);
}
void LEDProvider_Analog::show(void)
{
	_IsActive = true;
	setPixelColor(0, 0, 0, 0);
}

uint16_t LEDProvider_Analog::numPixels(void)
{
	return 1;
}

void LEDProvider_Analog::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	if (_IsActive)
	{
		analogWrite(_PinR, (uint8_t)(r * _CurrentBrightness));
		analogWrite(_PinG, (uint8_t)(g * _CurrentBrightness));
		analogWrite(_PinB, (uint8_t)(b * _CurrentBrightness));
	}
	else
	{
		analogWrite(_PinR, 0);
		analogWrite(_PinG, 0);
		analogWrite(_PinB, 0);
	}
}
void LEDProvider_Analog::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
	setPixelColor(n, r, g, b, w);
}
void LEDProvider_Analog::setPixelColor(uint16_t n, uint32_t c)
{
	setPixelColor(n, (uint8_t)(c >> 16), (uint8_t)(c >> 8), (uint8_t)c);
}
void LEDProvider_Analog::fill(uint32_t c, uint16_t first, uint16_t count)
{
	setPixelColor(1, c);
}
void LEDProvider_Analog::setBrightness(uint8_t b)
{
	_CurrentBrightness = b;
	setPixelColor(0, 0, 0, 0);
}
void LEDProvider_Analog::clear(void)
{
	_IsActive = false;
	setPixelColor(0, 0, 0, 0);
}
void LEDProvider_Analog::updateLength(uint16_t n)
{
	if (n == 0)
	{
		clear();
	}
	else
	{
		show();
	}
}
void LEDProvider_Analog::updateType(neoPixelType t)
{
}