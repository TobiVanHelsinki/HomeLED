#include "LEDProvider_Analog.h"

LEDProvider_Analog::LEDProvider_Analog(uint8_t r, uint8_t g, uint8_t b)
{
	Serial.println("analog ctor");
	_PinR = r;
	_PinG = g;
	_PinB = b;
}

void LEDProvider_Analog::begin(void)
{
	Serial.println("begin");
}
void LEDProvider_Analog::show(void)
{
	Serial.println("show");
}
void LEDProvider_Analog::setPin(uint16_t p)
{
	Serial.println("setPin");
}
uint16_t LEDProvider_Analog::numPixels(void)
{
	Serial.println("numPixels");
	return 1;
}

void LEDProvider_Analog::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	Serial.println("setPixelColor_rgb");
	analogWrite(_PinR, r);
	analogWrite(_PinG, g);
	analogWrite(_PinB, b);
}
void LEDProvider_Analog::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
	Serial.println("setPixelColor_rgbw");
	setPixelColor(n, r, g, b, w);
}
void LEDProvider_Analog::setPixelColor(uint16_t n, uint32_t c)
{
	Serial.println("setPixelColor_c");
	setPixelColor(n, (uint8_t)(c >> 16), (uint8_t)(c >> 8), (uint8_t)c);
}
void LEDProvider_Analog::fill(uint32_t c, uint16_t first, uint16_t count)
{
	Serial.println("fill");
}
void LEDProvider_Analog::setBrightness(uint8_t b)
{
	Serial.println("setBrightness");
}
void LEDProvider_Analog::clear(void)
{
	Serial.println("clear");
}
void LEDProvider_Analog::updateLength(uint16_t n)
{
	Serial.println("updateLength");
}
void LEDProvider_Analog::updateType(neoPixelType t)
{
	Serial.println("updateType");
}