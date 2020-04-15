#pragma once
#include "ILEDProvider.h"
class LEDProvider_Analog : public ILEDProvider
{
public:

	LEDProvider_Analog(uint8_t pinR, uint8_t pinG, uint8_t pinB);

	void              begin(void);
	void              show(void);
	uint16_t          numPixels(void);
	void              setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void              setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
	void              setPixelColor(uint16_t n, uint32_t c);
	void              fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0);
	void              setBrightness(uint8_t b);
	void              clear(void);
	void              updateLength(uint16_t n);
	void              updateType(neoPixelType t);

protected:
	uint8_t _PinR;
	uint8_t _PinG;
	uint8_t _PinB;

	uint8_t _CurrentColor_R;
	uint8_t _CurrentColor_G;
	uint8_t _CurrentColor_B;

	bool _IsActive = false;
	uint8_t _CurrentBrightness = 255;
};
