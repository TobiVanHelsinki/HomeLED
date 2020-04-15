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
	Update();
}
void LEDProvider_Analog::clear(void)
{
	_IsActive = false;
	Update();
}

uint16_t LEDProvider_Analog::numPixels(void)
{
	return 1;
}

void LEDProvider_Analog::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	_CurrentColor_R = r;
	_CurrentColor_G = g;
	_CurrentColor_B = b;
	Update();
}
void LEDProvider_Analog::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
	_CurrentColor_R = r;
	_CurrentColor_G = g;
	_CurrentColor_B = b;
	Update();
}
void LEDProvider_Analog::setPixelColor(uint16_t n, uint32_t c)
{
	_CurrentColor_R = (uint8_t)(c >> 16);
	_CurrentColor_G = (uint8_t)(c >> 8);
	_CurrentColor_B = (uint8_t)(c >> 0);
	Update();
}
void LEDProvider_Analog::fill(uint32_t c, uint16_t first, uint16_t count)
{
	setPixelColor(1, c);
}
void LEDProvider_Analog::setBrightness(uint8_t b)
{
	_CurrentBrightness = b;
	Update();
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

void LEDProvider_Analog::Update()
{
	if (_IsActive)
	{
		auto r = (uint8_t)(_CurrentColor_R * _CurrentBrightness / 255.0);
		auto g = (uint8_t)(_CurrentColor_G * _CurrentBrightness / 255.0);
		auto b = (uint8_t)(_CurrentColor_B * _CurrentBrightness / 255.0);
		Serial.println("Update Leds (r,g,b):");
		Serial.println(r);
		Serial.println(g);
		Serial.println(b);
		analogWrite(_PinR, r);
		analogWrite(_PinG, g);
		analogWrite(_PinB, b);
		//analogWrite(_PinG, (uint8_t)(_CurrentColor_G * _CurrentBrightness));
		//analogWrite(_PinB, (uint8_t)(_CurrentColor_B * _CurrentBrightness));
	}
	else
	{
		Serial.println("Update Leds: none");
		analogWrite(_PinR, 0);
		analogWrite(_PinG, 0);
		analogWrite(_PinB, 0);
	}
}