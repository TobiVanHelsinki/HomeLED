#include "Mode_Sin.h"
#include <math.h>

// the original static sin table was inspired from <https://github.com/adafruit/Adafruit_NeoPixel/blob/b6ebf749760f8e0dc7bbb1c12f4d46c06ff2b058/Adafruit_NeoPixel.cpp>

SinMode::SinMode(ILEDProvider* leds) : ColorMode(leds)
{
	BuildTable(DebugOutput);
}

 int SinMode::positive_modulo(int i, int n)
{
	return (i % n + n) % n;
}

void SinMode::NextState()
{
	for (int ledpos = 0; ledpos < leds->numPixels(); ledpos+=StepSize + Skip)
	{
		float scale = SinTable[positive_modulo((Multi * SinTabelSize <= 0 ? 1: Multi * SinTabelSize / 100) * (int)(ledpos / (double)StepSize) + timepos, SinTabelSize)];
		auto color = Adafruit_NeoPixel::Color(
			(int)(CurrentColor_r * scale),
			(int)(CurrentColor_g * scale),
			(int)(CurrentColor_b * scale),
			(int)(CurrentColor_a * scale)
		);
		if (DebugOutput && ledpos < 1)
		{
			auto colorstring = new char[6];
			sprintf(colorstring, "%06x", color);
			SERIALWRITELINE(String(scale) + " : " + String(colorstring));
		}
		for (size_t s = 0; s < StepSize; s++)
		{
			leds->setPixelColor(ledpos + s, color);
		}
	}
	timepos = positive_modulo(timepos + 1, SinTabelSize);
}

void SinMode::BuildTable(bool Verbose)
{
	SinTable = new float[SinTabelSize];
	float stepsize = TWO_PI / SinTabelSize;
	auto offset = 1.0f - Scaling;
	for (auto i = 0; i < SinTabelSize; i++)
	{
		SinTable[i] = (sin(i * stepsize - PI * 0.5) * Scaling) + offset;
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

String SinMode::HandleProperty(String Name, String Value)
{
	if (Name == "t")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&SinTabelSize, "SinTableSize", Value, 2, 1024);
			BuildTable(false);
		}
		return "t=" + String(SinTabelSize) + "&";
	}
	else if (Name == "mu")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&Multi, "Multiplikator", Value, 0, 120);
		}
		return "mu=" + String(Multi) + "&";
	}
	else if (Name == "s")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&Scaling, "Scaling", Value, 0.0, 0.5);
			BuildTable(false);
		}
		return "s=" + String(Scaling) + "&";
	}
	else if (Name == "build")
	{
		BuildTable(true);
		return "RebuildTable&";
	}
	return ColorMode::HandleProperty(Name, Value);
}
