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
	//names.push_back("secondcolor_h");
	//names.push_back("secondcolor_s");
	//names.push_back("secondcolor_v");
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
	//if (Name == "secondcolor_h")
	//{
	//	return String(SecondColor_h);
	//}
	//else if (Name == "secondcolor_s")
	//{
	//	return String(SecondColor_s);
	//}
	//else if (Name == "secondcolor_v")
	//{
	//	return String(SecondColor_v);
	//}
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
		//RefreshSecondColor();
		return result;
	}
	//if (Name == "secondcolor_h")
	//{
	//	auto result = SetinBoundsAndReport(&SecondColor_h, "Second Hue", Value);
	//	RefreshSecondColor();
	//	return result;
	//}
	//else if (Name == "secondcolor_s")
	//{
	//	auto result = SetinBoundsAndReport(&SecondColor_s, "Second Saturation", Value);
	//	RefreshSecondColor();
	//	return result;
	//}
	//else if (Name == "secondcolor_v")
	//{
	//	auto result = SetinBoundsAndReport(&SecondColor_v, "Second Value", Value);
	//	RefreshSecondColor();
	//	RefreshColorParts();
	//	return result;
	//}
	else
	{
		return ColorMode::Set(Name, Value);
	}
}

void TwoColorMode::RefreshSecondColor()
{
	CurrentColor_r = CurrentColor >> 16;
	CurrentColor_g = CurrentColor >> 8;
	CurrentColor_b = CurrentColor >> 0;
	CalculateHSV(CurrentColor_r, CurrentColor_g, CurrentColor_b);
}