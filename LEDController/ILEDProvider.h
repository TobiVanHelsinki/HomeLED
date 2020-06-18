#pragma once
#include <Adafruit_NeoPixel.h>
#include "ProjectHeader.h"
class ILEDProvider
{
public:
	virtual void              begin(void) { };
	virtual void              show(void) { };
	virtual uint16_t          numPixels(void) { return 0; };
	virtual void              setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) { };
	virtual void              setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) { };
	virtual void              setPixelColor(uint16_t n, uint32_t c) { };
	virtual void              fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0) { };
	virtual void              setBrightness(uint8_t b) { };
	virtual void              clear(void) { };
	virtual void              updateLength(uint16_t n) { };
	virtual void              updateType(neoPixelType t) { };
};
