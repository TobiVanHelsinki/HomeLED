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
	names.push_back("sk");
	names.push_back("st");
	return names;
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
	if (Name == "sk")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&Skip, "sk", Value, 0, 10);
			leds->clear();
		}
		return "sk=" + String(Skip) + "&";
	}
	else if (Name == "st")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&StepSize, "st", Value, 1);
		}
		return "st=" + String(StepSize) + "&";
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
		return "Set " + name + " to " + String((*savePlace)) + "\n";
	}
	else
	{
		return String();
	}
}
String ModeBase::SetinBoundsAndReport(uint32_t* savePlace, String name, String parameter, uint32_t min, uint32_t max)
{
	uint32_t newVal = static_cast<uint32_t>(std::atoll(parameter.c_str())); // convert to long long, because the lib function toint does not can do 32bit unsigned
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
uint32_t ModeBase::Wheel(uint16_t WheelPos, byte White) //TODO introduce relative width of the wheel
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3, White);
	}
	if (WheelPos < 170)
	{
		WheelPos -= 85;
		return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3, White);
	}
	WheelPos -= 170;
	return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0, White);
}