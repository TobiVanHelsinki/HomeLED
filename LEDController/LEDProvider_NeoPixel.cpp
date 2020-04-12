#include "LEDProvider_NeoPixel.h"

LEDProvider_NeoPixel::LEDProvider_NeoPixel(Adafruit_NeoPixel* adaptee)
{
	_Adaptee = adaptee;
}
void LEDProvider_NeoPixel::begin(void)
{
	return _Adaptee->begin();
}
void LEDProvider_NeoPixel::show(void)
{
	return _Adaptee->show();
}
void LEDProvider_NeoPixel::setPin(uint16_t p)
{
	return _Adaptee->setPin(p);
}
void LEDProvider_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	return _Adaptee->setPixelColor(n, r, g, b);
}
void LEDProvider_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
	return _Adaptee->setPixelColor(n, r, g, b, w);
}
void LEDProvider_NeoPixel::setPixelColor(uint16_t n, uint32_t c)
{
	return _Adaptee->setPixelColor(n, c);
}
void LEDProvider_NeoPixel::fill(uint32_t c, uint16_t first, uint16_t count)
{
	return _Adaptee->fill(c, first, count);
}
void LEDProvider_NeoPixel::setBrightness(uint8_t b)
{
	return _Adaptee->setBrightness(b);
}
void LEDProvider_NeoPixel::clear(void)
{
	return _Adaptee->clear();
}
void LEDProvider_NeoPixel::updateLength(uint16_t n)
{
	return _Adaptee->updateLength(n);
}
void LEDProvider_NeoPixel::updateType(neoPixelType t)
{
	return _Adaptee->updateType(t);
}
uint16_t LEDProvider_NeoPixel::numPixels(void)
{
	return _Adaptee->numPixels();
}