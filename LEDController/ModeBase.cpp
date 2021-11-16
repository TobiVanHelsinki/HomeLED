#include "ModeBase.h"

ModeBase::ModeBase(ILEDProvider* ledstouse)
{
	delay(1);
	leds = ledstouse;
}
ModeBase::~ModeBase()
{ }

std::vector<String> ModeBase::ParameterNames()
{
	std::vector<String> names;
	names.push_back("d");
	return names;
}

String ModeBase::Get(String Name)
{
	return HandleProperty(Name, "");
}

String ModeBase::HandleProperty(String Name, String Value)
{
	if (Name == "d")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&DebugOutput, "d", Value);
		}
		return "d=" + String(DebugOutput) + "&";
	}
	else
	{
		return String();
	}
}

String ModeBase::SetinBoundsAndReport(int* savePlace, String name, String parameter, int min, int max)
{
	auto newVal = parameter.toInt();
	if (newVal != (*savePlace))
	{
		if (newVal > max)
		{
			newVal = max;
		}
		else if (newVal < min)
		{
			newVal = min;
		}
		(*savePlace) = newVal;
		return "Set " + name + " to " + String((*savePlace)) + "\n";
	}
	else
	{
		return String();
	}
}

String ModeBase::SetinBoundsAndReport(uint8_t* savePlace, String name, String parameter, uint8_t min, uint8_t max)
{
	auto newVal = parameter.toInt();
	if (newVal != (*savePlace))
	{
		if (newVal > max)
		{
			newVal = max;
		}
		else if (newVal < min)
		{
			newVal = min;
		}
		(*savePlace) = newVal;
		SERIALWRITELINE("Set " + name + " to " + String((*savePlace)) + "\n");
		return "Set " + name + " to " + String((*savePlace)) + "\n";
	}
	else
	{
		return String();
	}
}
String ModeBase::SetinBoundsAndReport(uint32_t* savePlace, String name, String parameter, uint32_t min, uint32_t max)
{
	uint32 newVal = parameter.toInt();
	if (newVal != (*savePlace))
	{
		if (newVal > max)
		{
			newVal = max;
		}
		else if (newVal < min)
		{
			newVal = min;
		}
		(*savePlace) = newVal;
		return "Set " + name + " to " + String((*savePlace)) + "\n";
	}
	else
	{
		return String();
	}
}
String ModeBase::SetinBoundsAndReport(uint16_t* savePlace, String name, String parameter, uint16_t min, uint16_t max)
{
	auto newVal = parameter.toInt();
	if (newVal != (*savePlace))
	{
		if (newVal > max)
		{
			newVal = max;
		}
		else if (newVal < min)
		{
			newVal = min;
		}
		(*savePlace) = newVal;
		return "Set " + name + " to " + String((*savePlace)) + "\n";
	}
	else
	{
		return String();
	}
}
String ModeBase::SetinBoundsAndReport(float* savePlace, String name, String parameter, float min, float max)
{
	auto newVal = parameter.toFloat();
	if (newVal != (*savePlace))
	{
		if (newVal > max)
		{
			newVal = max;
		}
		else if (newVal < min)
		{
			newVal = min;
		}
		(*savePlace) = newVal;
		return "Set " + name + " to " + String((*savePlace)) + "\n";
	}
	else
	{
		return String();
	}
}
String ModeBase::SetinBoundsAndReport(bool* savePlace, String name, String parameter)
{
	(*savePlace) = parameter.toInt();
	return ((*savePlace) ? "Set " : "Unset ") + name + " Mode";
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ModeBase::Wheel(byte WheelPos) //TODO introduce relative width of the wheel
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170)
	{
		WheelPos -= 85;
		return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}