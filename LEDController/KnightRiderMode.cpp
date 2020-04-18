#include "KnightRiderMode.h"
#include "math.h"
KnightRiderMode::KnightRiderMode(ILEDProvider* leds) : ColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(255, 0, 0);
}

void KnightRiderMode::NextState()
{
	auto BackgroundColor = Adafruit_NeoPixel::Color(0, 0, 128);
	if (CurrentPosition >= leds->numPixels())
	{
		CurrentDirection = !CurrentDirection;
		CurrentPosition = 0;
	}
	for (size_t i = 0; i < leds->numPixels(); i++)
	{
		leds->setPixelColor(i, BackgroundColor);
	}
	PrintTail(CurrentPosition);
	CurrentPosition++;
}

void KnightRiderMode::PrintTail(size_t startPos)
{
	auto max = leds->numPixels();
	for (size_t i = 0; i < Width; i++)
	{
		auto pixelpos = startPos + i;
		if (pixelpos >= max)
		{
			pixelpos = max - (pixelpos - max);
		}
		leds->setPixelColor(CalculateMirroredPosition(pixelpos), CalculateFringedColor(i));
	}
}
uint32_t KnightRiderMode::CalculateFringedColor(size_t innerPos)
{
	auto max = CurrentColor_v;
	auto intense = 0.0f;
	if (Fringe <= 0 || innerPos >= Fringe)
	{
		intense = max;
	}
	else
	{
		intense = Fringe * max / (float)innerPos;
	}
	return Adafruit_NeoPixel::ColorHSV(CurrentColor_h, CurrentColor_s, (uint16_t)CurrentColor_v * intense);
}
size_t KnightRiderMode::CalculateMirroredPosition(size_t pos)
{
	return CurrentDirection ? pos : leds->numPixels() - 1 - pos;
}

String KnightRiderMode::ID = "knightrider";

String KnightRiderMode::GetID()
{
	return ID;
}

std::vector<String> KnightRiderMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("width");
	names.push_back("fringe");
	auto baseNames = ColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String KnightRiderMode::Get(String Name)
{
	if (Name == "width")
	{
		return String(Width);
	}
	else if (Name == "fringe")
	{
		return String(Fringe);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}

String KnightRiderMode::Set(String Name, String Value)
{
	if (Name == "width")
	{
		return SetinBoundsAndReport(&Width, "Width", Value);
	}
	else if (Name == "fringe")
	{
		return SetinBoundsAndReport(&Fringe, "Fringe", Value);
	}
	return ColorMode::Set(Name, Value);
}