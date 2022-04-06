#include "Mode_Rainbow.h"

RainbowMode::RainbowMode(ILEDProvider* leds) : SinMode(leds)
{
	BuildTable(DebugOutput);
}

String RainbowMode::ID = "rainbow";

String RainbowMode::GetID()
{
	return ID;
}


void RainbowMode::NextState()
{
	for (int ledpos = 0; ledpos < leds->numPixels(); ledpos += StepSize + Skip)
	{
		// a number including between 0 and 1
		float scale = 
			(
				static_cast<float>
				(
					(int)
					(
						1 //(Multi * SinTabelSize == 0 ? 1 : Multi * SinTabelSize) // just why wont it work if Multi is 0
						
						* (float)ledpos
						/ (float)StepSize
						+ timepos
					) 
					% SinTabelSize
				)
			)
			/ SinTabelSize;
		//float scale = SinTable[(Multi * (int)(ledpos / (double)StepSize) + timepos) % SinTabelSize];
		auto color = Adafruit_NeoPixel::ColorHSV(scale * 65535, 255, CurrentColor_v) | CurrentColor_a << 24;
		if (DebugOutput && ledpos < 1)
		{
			auto colorstring = new char[6];
			sprintf(colorstring, "%06x", color);
			SERIALWRITELINE(String(scale) + " : " + String(colorstring));
		}

		for (size_t s = 0; s < StepSize; s++)
		{
			leds->setPixelColor(ledpos + s, color);
		}
	}
	timepos = positive_modulo(timepos + 1, SinTabelSize);
}
