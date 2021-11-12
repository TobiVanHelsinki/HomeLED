#include "SinMode.h"
#include <math.h>
SinMode::SinMode(ILEDProvider* leds) : ColorMode(leds)
{
	BuildTable(DebugOutput);
}

inline int SinMode::positive_modulo(int i, int n)
{
	return (i % n + n) % n;
}

void SinMode::NextState()
{
	for (int ledpos = 0; ledpos < leds->numPixels(); ledpos++)
	{
		float scale = SinTable[positive_modulo(Multi * ledpos + timepos, SinTabelSize)];
		auto color = Adafruit_NeoPixel::Color(
			(int)(CurrentColor_r * scale),
			(int)(CurrentColor_g * scale),
			(int)(CurrentColor_b * scale)
		);
		if (DebugOutput && ledpos < 10)
		{
			auto colorstring = new char[6];
			sprintf(colorstring, "%06x", color);
			SERIALWRITELINE(String(scale) + " : " + String(colorstring));
		}
		leds->setPixelColor(ledpos, color);
	}
	timepos = positive_modulo(timepos + 1, SinTabelSize);
}

void SinMode::BuildTable(bool Verbose)
{
	SinTable = new float[SinTabelSize];
	float stepsize = TWO_PI / SinTabelSize;
	auto Offset = 1.0f - Scaling;
	for (auto i = 0; i < SinTabelSize; i++)
	{
		SinTable[i] = (sin(i * stepsize - PI * 0.5) * Scaling) + Offset;
	}
	if (Verbose)
	{
		SERIALWRITELINE("SinTable: ");
		for (auto i = 0; i < SinTabelSize; i++)
		{
			SERIALWRITELINE(String(SinTable[i]));
		}
		SERIALWRITELINE("SinTable");
	}
}

String SinMode::ID = "sin";

String SinMode::GetID()
{
	return ID;
}

std::vector<String> SinMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("t");
	names.push_back("mu");
	names.push_back("s");
	names.push_back("build");
	auto baseNames = ColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String SinMode::Get(String Name)
{
	if (Name == "t")
	{
		return String(SinTabelSize);
	}
	else if (Name == "mu")
	{
		return String(Multi);
	}
	else if (Name == "s")
	{
		return String(Scaling);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}

String SinMode::Set(String Name, String Value)
{
	if (Name == "t")
	{
		auto oldsize = SinTabelSize;
		auto result = SetinBoundsAndReport(&SinTabelSize, "SinTabelSize", Value, 2, 1024);
		BuildTable(true);
		return result;
	}
	else if (Name == "mu")
	{
		return SetinBoundsAndReport(&Multi, "Multiplikator", Value, 0, 1024);
	}
	else if (Name == "s")
	{
		auto result = SetinBoundsAndReport(&Scaling, "Scaling", Value, 0.0, 0.5);
		BuildTable(true);
		return result;
	}
	else if (Name == "build")
	{
		BuildTable(true);
		return "Rebuild Table";
	}
	return ColorMode::Set(Name, Value);
}