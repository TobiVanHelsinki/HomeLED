#include "TwoColorMode.h"

#include "math.h"
TwoColorMode::TwoColorMode(ILEDProvider* leds) : ColorMode(leds)
{
	SecondColor = Adafruit_NeoPixel::Color(0, 10, 50);
}

std::vector<String> TwoColorMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("secondcolor");
	auto baseNames = ColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String TwoColorMode::Get(String Name)
{
	if (Name == "secondcolor")
	{
		return String(SecondColor);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}

String TwoColorMode::Set(String Name, String Value)
{
	if (Name == "secondcolor")
	{
		auto result = SetinBoundsAndReport(&SecondColor, "Second Color", Value);
		CalculateRGB(SecondColor, &SecondColor_r, &SecondColor_g, &SecondColor_b);
		CalculateHSV(SecondColor_r, SecondColor_g, SecondColor_b, &SecondColor_h, &SecondColor_s, &SecondColor_v);
		return result;
	}
	else
	{
		return ColorMode::Set(Name, Value);
	}
}