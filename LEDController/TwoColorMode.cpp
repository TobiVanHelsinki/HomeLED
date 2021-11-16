#include "TwoColorMode.h"

#include "math.h"
TwoColorMode::TwoColorMode(ILEDProvider* leds) : ColorMode(leds)
{
	SecondColor = Adafruit_NeoPixel::Color(0, 10, 50);
	RefreshSecondColors();
}

void TwoColorMode::NextState()
{
	for (uint16_t i = 0; i < leds->numPixels(); i++)
	{
		if (rand() % 2 == 0)
		{
			leds->setPixelColor(i, CurrentColor);
		}
		else
		{
			leds->setPixelColor(i, SecondColor);
		}
	}
}

String TwoColorMode::ID = "c2";

String TwoColorMode::GetID()
{
	return ID;
}

std::vector<String> TwoColorMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("c2");
	auto baseNames = ColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String TwoColorMode::Get(String Name)
{
	return HandleProperty(Name, "");
	if (Name == "c2")
	{
		return String(SecondColor);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}

String TwoColorMode::HandleProperty(String Name, String Value)
{
	if (Name == "c2")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&SecondColor, "Second Color", Value);
			RefreshSecondColors();
		}
		return "c2=" + String(SecondColor) + "&";
	}
	else
	{
		return ColorMode::HandleProperty(Name, Value);
	}
}

void TwoColorMode::RefreshSecondColors()
{
	CalculateRGB(SecondColor, &SecondColor_r, &SecondColor_g, &SecondColor_b);
	CalculateHSV(SecondColor_r, SecondColor_g, SecondColor_b, &SecondColor_h, &SecondColor_s, &SecondColor_v);
}