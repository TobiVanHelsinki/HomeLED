#pragma once
#include "ILEDProvider.h"
/// <summary>
/// This an adapter for NeoPixel.
/// </summary>
/// <seealso cref="Adafruit_NeoPixel" />
/// <seealso cref="ILEDProvider" />
class LEDProvider_NeoPixel : public ILEDProvider
{
public:
	LEDProvider_NeoPixel(Adafruit_NeoPixel* adaptee);

	void              begin(void);
	void              show(void);
	void              setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void              setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
	void              setPixelColor(uint16_t n, uint32_t c);
	void              fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0);
	void              setBrightness(uint8_t b);
	void              clear(void);
	void              updateLength(uint16_t n);
	void              updateType(neoPixelType t);
	uint16_t		  numPixels(void);

protected:
	Adafruit_NeoPixel* _Adaptee;
};
