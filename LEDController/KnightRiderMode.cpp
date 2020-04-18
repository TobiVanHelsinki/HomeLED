#include "KnightRiderMode.h"
#include "math.h"
KnightRiderMode::KnightRiderMode(ILEDProvider* leds) : TwoColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(255, 0, 0);
	SecondColor = Adafruit_NeoPixel::Color(255, 0, 0);
}

void KnightRiderMode::NextState()
{
	if (CurrentPosition >= leds->numPixels())
	{
		CurrentDirection = !CurrentDirection;
		CurrentPosition = 0;
	}
	for (size_t i = 0; i < leds->numPixels(); i++)
	{
		leds->setPixelColor(i, SecondColor);
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

//innerPos from 0 to Width
uint32_t KnightRiderMode::CalculateFringedColor(size_t innerPos)
{
	auto max = CurrentColor_v;
	auto relative_v = 0.0f;
	if (Fringe <= 0 || innerPos >= Fringe)
	{
		relative_v = max;
	}
	else
	{
		relative_v = max * (float)innerPos / Fringe;
	}
	if (DebugOutput)
	{
		Serial.print("relative_v at ");
		Serial.print(innerPos);
		Serial.print(":");
		Serial.println(relative_v);
	}
	return Adafruit_NeoPixel::ColorHSV(CurrentColor_h, CurrentColor_s, (uint16_t)relative_v);
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
	auto baseNames = TwoColorMode::ParameterNames();
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
		return TwoColorMode::Get(Name);
	}
}

String KnightRiderMode::Set(String Name, String Value)
{
	if (Name == "width")
	{
		if (Fringe > Value.toInt())
		{
			Fringe = Value.toInt();
		}
		return SetinBoundsAndReport(&Width, "Width", Value);
	}
	else if (Name == "fringe")
	{
		if (Value.toInt() > Width)
		{
			Value = String(Width);
		}
		return SetinBoundsAndReport(&Fringe, "Fringe", Value);
	}
	return TwoColorMode::Set(Name, Value);
}