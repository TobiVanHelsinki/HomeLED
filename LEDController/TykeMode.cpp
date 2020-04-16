#include "TykeMode.h"
TykeMode::TykeMode(ILEDProvider* leds) : ColorMode(leds)
{
}

void TykeMode::NextState()
{
	if (Position > leds->numPixels())
	{
		Position = 0;
	}
	else
	{
		Position++;
	}
	leds->setPixelColor(Position, ColorLeft);         //  Set pixel's color (in RAM)
	leds->setPixelColor(leds->numPixels() - Position, ColorRight);        //  Set pixel's color (in RAM)
}

String TykeMode::ID = "tyke";

String TykeMode::GetID()
{
	return ID;
}

std::vector<String> TykeMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("ColorLeft");
	names.push_back("ColorRight");
	auto baseNames = ColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String TykeMode::Get(String Name)
{
	if (Name == "ColorLeft")
	{
		return String(ColorLeft);
	}
	else if (Name == "ColorRight")
	{
		return String(ColorRight);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}

String TykeMode::Set(String Name, String Value)
{
	if (Name == "ColorLeft")
	{
		return SetinBoundsAndReport(&ColorLeft, "ColorLeft", Value);
	}
	else if (Name == "ColorRight")
	{
		return SetinBoundsAndReport(&ColorRight, "ColorRight", Value);
	}
	return ColorMode::Set(Name, Value);
}