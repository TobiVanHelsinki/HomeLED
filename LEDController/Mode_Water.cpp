#include "Mode_Water.h"
#include "math.h"
Mode_Water::Mode_Water(ILEDProvider* leds) : TwoColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(51, 187, 255);
	SecondColor = Adafruit_NeoPixel::Color(0, 0, 102);
}

void Mode_Water::NextState()
{
	
}

String Mode_Water::ID = "water";

String Mode_Water::GetID()
{
	return ID;
}

std::vector<String> Mode_Water::ParameterNames()
{
	std::vector<String> names;
	names.push_back("a");
	auto baseNames = TwoColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String Mode_Water::HandleProperty(String Name, String Value)
{
	if (Name == "a")
	{
		if (!Value.isEmpty())
		{
			Areas = Value; //TODO Sanity checks
		}
		return "a=" + Areas + "&";
	}
	return TwoColorMode::HandleProperty(Name, Value);
}