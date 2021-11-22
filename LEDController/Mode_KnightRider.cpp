#include "Mode_KnightRider.h"
#include "math.h"
KnightRiderMode::KnightRiderMode(ILEDProvider* leds) : TwoColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(255, 0, 0);
	SecondColor = Adafruit_NeoPixel::Color(0, 0, 0);
}

void KnightRiderMode::NextState()
{
	leds->clear();
	if (CurrentPosition >= leds->numPixels())
	{
		CurrentDirection = !CurrentDirection;
		CurrentPosition = 0;
	}
	for (size_t i = 0; i < leds->numPixels(); i += StepSize)
	{
		leds->setPixelColor(i, SecondColor);
	}
	PrintTail(CurrentPosition);
	CurrentPosition += StepSize /*+ Skip*/; //skip hier zu kompliziert
}

void KnightRiderMode::PrintTail(size_t startPos)
{
	auto max = leds->numPixels();
	for (auto i = 0; i < Width; i++)
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
		SERIALWRITE("relative_v at ");
		SERIALWRITE(innerPos);
		SERIALWRITE(":");
		SERIALWRITELINE(relative_v);
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
	names.push_back("w");
	names.push_back("f");
	auto baseNames = TwoColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String KnightRiderMode::HandleProperty(String Name, String Value)
{
	if (Name == "w")
	{
		if (!Value.isEmpty())
		{
			uint val = Value.toInt();
			if (Fringe > val)
			{
				Fringe = val;
			}
			SetinBoundsAndReport(&Width, "Width ", Value);
		}
		return "w=" + String(Width) + "&";
	}
	else if (Name == "f")
	{
		if (!Value.isEmpty())
		{
			if (Value.toInt() > Width)
			{
				Value = String(Width);
			}
			SetinBoundsAndReport(&Fringe, "Fringe", Value);
		}
		return  "f=" + String(Fringe) + "&";
	}
	return TwoColorMode::HandleProperty(Name, Value);
}