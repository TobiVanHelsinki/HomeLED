#include "Mode_Rainbow.h"

RainbowMode::RainbowMode(ILEDProvider* leds) : ModeBase(leds)
{
}

String RainbowMode::ID = "rainbow";

String RainbowMode::GetID()
{
	return ID;
}

void RainbowMode::NextState()
{
	for (int i = 0; i < leds->numPixels(); i += Skip)
	{
		int x;
		if (DisturbingMode)
		{
			x = (i * 256 / leds->numPixels());
		}
		else
		{
			x = i;
		}
		leds->setPixelColor(i, Wheel((x + j) & 255));
	}
	j = (j + 1) % 256;
}

std::vector<String> RainbowMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("r");
	auto baseNames = ModeBase::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String RainbowMode::Get(String Name)
{
	return HandleProperty(Name, "");
	if (Name == "r")
	{
		return String(DisturbingMode);
	}
	else
	{
		return ModeBase::Get(Name);
	}
}

String RainbowMode::HandleProperty(String Name, String Value)
{
	if (Name == "r")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&DisturbingMode, "DisturbingMode", Value);
		}
		return "r=" + String(DisturbingMode) + "&";
	}
	else
	{
		return ModeBase::HandleProperty(Name, Value);
	}
}