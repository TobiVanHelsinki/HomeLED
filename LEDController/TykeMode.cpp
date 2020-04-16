#include "TykeMode.h"
TykeMode::TykeMode(ILEDProvider* leds) : ColorMode(leds)
{
}

void TykeMode::NextState()
{
	if (Position < leds->numPixels())
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

String TykeMode::GetID()
{
	return "tyke";
}

String TykeMode::Set(String Name, String Value)
{
	if (Name == "ColorLeft")
	{
		auto newval = Value.toInt();
		if (newval != ColorLeft)
		{
			ColorLeft = newval;
			return "Set ColorLeft to " + String(ColorLeft) + "\n";
		}
	}
	else if (Name == "ColorRight")
	{
		auto newval = Value.toInt();
		if (newval != ColorRight)
		{
			ColorRight = newval;
			return "Set ColorRight to " + String(ColorRight) + "\n";
		}
	}
	return ColorMode::Set(Name, Value);
}
int TykeMode::NumberofParams()
{
	return 2 + ColorMode::NumberofParams();
}
String TykeMode::GetName(int Number)
{
	auto lastno = ColorMode::NumberofParams();
	if (Number == 0 + lastno)
	{
		return "ColorLeft";
	}
	else if (Number == 1 + lastno)
	{
		return "ColorRight";
	}
	else
	{
		return ColorMode::GetName(Number);
	}
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