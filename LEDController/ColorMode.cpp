#include "ColorMode.h"

ColorMode::ColorMode(ILEDProvider* leds) : ModeBase(leds)
{
}

void ColorMode::NextState()
{
	for (uint16_t i = 0; i < leds->numPixels(); i++)
	{
		leds->setPixelColor(i, CurrentColor);
	}
}

String ColorMode::ID = "color";

String ColorMode::GetID()
{
	return ID;
}

std::vector<String> ColorMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("color_h");
	names.push_back("color_s");
	names.push_back("color_v");
	auto baseNames = ModeBase::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String ColorMode::Get(String Name)
{
	if (Name == "color_h")
	{
		return String(CurrentColor_h);
	}
	else if (Name == "color_s")
	{
		return String(CurrentColor_s);
	}
	else if (Name == "color_v")
	{
		return String(CurrentColor_v);
	}
	else
	{
		return ModeBase::Get(Name);
	}
}

String ColorMode::Set(String Name, String Value)
{
	if (Name == "color_h")
	{
		auto result = SetinBoundsAndReport(&CurrentColor_h, "Hue", Value);
		RefreshColor();
		return result;
	}
	else if (Name == "color_s")
	{
		auto result = SetinBoundsAndReport(&CurrentColor_s, "Saturation", Value);
		RefreshColor();
		return result;
	}
	else if (Name == "color_v")
	{
		auto result = SetinBoundsAndReport(&CurrentColor_v, "Value ", Value);
		RefreshColor();
		return result;
	}
	return ModeBase::Set(Name, Value);
}

void ColorMode::RefreshColor()
{
	CurrentColor = Adafruit_NeoPixel::ColorHSV(CurrentColor_h, CurrentColor_s, CurrentColor_h);
	CurrentColor_r = CurrentColor << 16;
	CurrentColor_g = CurrentColor << 8;
	CurrentColor_b = CurrentColor << 0;
}