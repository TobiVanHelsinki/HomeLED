#include "ColorMode.h"

ColorMode::ColorMode(Adafruit_NeoPixel* leds) : ModeBase(leds)
{
}

void ColorMode::NextState()
{
	for (uint16_t i = 0; i < leds->numPixels(); i++)
	{
		leds->setPixelColor(i, CurrentColor);
	}
}

String ColorMode::ID()
{
	return "color";
}

String ColorMode::Set(String Name, String Value)
{
	if (Name == "r")
	{
		auto newval = Value.toInt();
		if (CurrentColor_r != newval)
		{
			CurrentColor_r = newval;
			CurrentColor |= leds->Color(newval, 0, 0);
			return "Set red to  " + String(CurrentColor_r) + "\n";
		}
	}
	else if (Name == "g")
	{
		auto newval = Value.toInt();
		if (CurrentColor_g != newval)
		{
			CurrentColor_g = newval;
			CurrentColor |= leds->Color(0, newval, 0);
			return "Set green to " + String(CurrentColor_g) + "\n";
		}
	}
	else if (Name == "b")
	{
		auto newval = Value.toInt();
		if (CurrentColor_b != newval)
		{
			CurrentColor_b = newval;
			CurrentColor |= leds->Color(0, 0, newval);
			return "Set blue to " + String(CurrentColor_b) + "\n";
		}
	}
	else if (Name == "col")
	{
		auto newval = Value.toInt();
		if (CurrentColor != newval)
		{
			CurrentColor = newval;
			return "Set col to " + String(CurrentColor) + "\n";
		}
	}
	return ModeBase::Set(Name, Value);
}
int ColorMode::NumberofParams()
{
	return 3 + ModeBase::NumberofParams();
}
String ColorMode::GetName(int Number)
{
	auto lastno = ModeBase::NumberofParams();
	if (Number == 0 + lastno)
	{
		return "r";
	}
	else if (Number == 1 + lastno)
	{
		return "g";
	}
	else if (Number == 2 + lastno)
	{
		return "b";
	}
	else
	{
		return ModeBase::GetName(Number);
	}
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