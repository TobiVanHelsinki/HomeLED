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
	names.push_back("r");
	names.push_back("g");
	names.push_back("b");
	auto baseNames = ModeBase::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String ColorMode::Get(String Name)
{
	if (Name == "r")
	{
		return String(CurrentColor_r);
	}
	else if (Name == "g")
	{
		return String(CurrentColor_g);
	}
	else if (Name == "b")
	{
		return String(CurrentColor_b);
	}
	else
	{
		return ModeBase::Get(Name);
	}
}

String ColorMode::Set(String Name, String Value)
{
	if (Name == "r")
	{
		auto result = SetinBoundsAndReport(&CurrentColor_r, "Red", Value);
		RefreshColor();
		return result;
	}
	else if (Name == "g")
	{
		auto result = SetinBoundsAndReport(&CurrentColor_g, "Green", Value);
		RefreshColor();
		return result;
	}
	else if (Name == "b")
	{
		auto result = SetinBoundsAndReport(&CurrentColor_b, "Blue", Value);
		RefreshColor();
		return result;
	}
	else if (Name == "col")
	{
		return SetinBoundsAndReport(&CurrentColor, "Color", Value);
	}
	return ModeBase::Set(Name, Value);
}

void ColorMode::RefreshColor()
{
	CurrentColor = Adafruit_NeoPixel::Color(
		(int)(CurrentColor_r),
		(int)(CurrentColor_g),
		(int)(CurrentColor_b)
	);
}