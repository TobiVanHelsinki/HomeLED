#include "SinMode.h"
#include <math.h>
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
	for (int ledpos = 0; ledpos < leds->numPixels(); ledpos+=StepSize)
	{
		float scale = SinTable[positive_modulo(Multi * (int)(ledpos/(double)StepSize) + timepos, SinTabelSize)];
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
	names.push_back("st");
	names.push_back("build");
	auto baseNames = ColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String SinMode::Get(String Name) //TODO später Get und handle in allen modes zusammenführen
{
	return HandleProperty(Name, "");
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
			SetinBoundsAndReport(&Multi, "Multiplikator", Value, 0, 1024);
			BuildTable(false);
		}
		return "mu=" + String(Multi) + "&";
	}
	else if (Name == "s")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&Scaling, "Scaling", Value, 0.0, 1);
			BuildTable(false);
		}
		return "s=" + String(Scaling) + "&";
	}
	else if (Name == "st")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&StepSize, "", Value, 1, 1024);
		}
		return "st=" + String(StepSize) + "&";
	}
	else if (Name == "build")
	{
		BuildTable(true);
		return "RebuildTable&";
	}
	return ColorMode::HandleProperty(Name, Value);
}