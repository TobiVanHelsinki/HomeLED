#include "RainbowMode.h"

RainbowMode::RainbowMode(ILEDProvider* leds) : ModeBase(leds)
{
}

String RainbowMode::ID()
{
	return "rainbow";
}

void RainbowMode::NextState()
{
	for (int i = 0; i < leds->numPixels(); i++)
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

String RainbowMode::Set(String Name, String Value)
{
	if (Name == "rand")
	{
		DisturbingMode = Value.toInt();
		if (DisturbingMode)
		{
			return "Set Random Mode";
		}
		else
		{
			return "Unset Random Mode";
		}
	}
	else
	{
		return ModeBase::Set(Name, Value);
	}
}

int RainbowMode::NumberofParams()
{
	return 1 + ModeBase::NumberofParams();
}
String RainbowMode::GetName(int Number)
{
	if (Number == 0)
	{
		return "rand";
	}
	else
	{
		return ModeBase::GetName(Number);
	}
}
String RainbowMode::Get(String Name)
{
	char s[10];
	if (Name == "rand")
	{
		sprintf(s, "$d", DisturbingMode);
		return s;
	}
	else
	{
		return ModeBase::Get(Name);
	}
}