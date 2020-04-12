#include "SinMode.h"
#include <math.h>
SinMode::SinMode(ILEDProvider* leds) : ColorMode(leds)
{
	BuildTable(DebugOutput);
}

inline int SinMode::positive_modulo(int i, int n) {
	return (i % n + n) % n;
}

void SinMode::NextState()
{
	for (int i = 0; i < leds->numPixels(); i++)
	{
		float scale = SinTable[positive_modulo(VerticalOffset * i + j, SinTabelSize)];
		auto color = Adafruit_NeoPixel::Color(
			(int)(CurrentColor_r * scale),
			(int)(CurrentColor_g * scale),
			(int)(CurrentColor_b * scale)
		);
		if (DebugOutput && i == 0)
		{
			auto colorstring = new char[6];
			sprintf(colorstring, "%06x", color);
			Serial.println(String(scale) + " : " + String(colorstring));
		}
		leds->setPixelColor(i, color);
	}
	j = positive_modulo(j + HorizontalOffset, SinTabelSize);
}

void SinMode::BuildTable(bool Verbose)
{
	SinTable = new float[SinTabelSize];
	float stepsize = TWO_PI / SinTabelSize;
	auto Offset = 1.0f - Scaling;
	for (size_t i = 0; i < SinTabelSize; i++)
	{
		SinTable[i] = (sin(i * stepsize - PI * 0.5) * Scaling) + Offset;
	}
	if (Verbose)
	{
		Serial.println("SinTable: ");
		for (size_t i = 0; i < SinTabelSize; i++)
		{
			Serial.println(String(SinTable[i]));
		}
		Serial.println("SinTable");
	}
}

String SinMode::ID()
{
	return "sin";
}

String SinMode::Set(String Name, String Value)
{
	if (Name == "ho")
	{
		auto newval = Value.toInt();
		if (newval != HorizontalOffset)
		{
			HorizontalOffset = newval;
			return "Set HorizontalOffset to " + String(HorizontalOffset) + "\n";
		}
	}
	else if (Name == "vo")
	{
		auto newval = Value.toInt();
		if (newval != VerticalOffset)
		{
			VerticalOffset = newval;
			return "Set VerticalOffset to " + String(VerticalOffset) + "\n";
		}
	}
	else if (Name == "scale")
	{
		auto value = Value.toFloat();
		if (value > 0.5)
		{
			Scaling = 0.5;
		}
		else if (value < 0.0)
		{
			Scaling = 0.0;
		}
		else
		{
			Scaling = value;
		}
		BuildTable(true);
		return "Set Scaling to " + String(Scaling);
	}
	else if (Name == "debug")
	{
		DebugOutput = Value.toInt();
		return DebugOutput ? "Debug active" : "Debug inactive";
	}
	else if (Name == "build")
	{
		BuildTable(true);
		return "Rebuild Table";
	}
	return ColorMode::Set(Name, Value);
}
int SinMode::NumberofParams()
{
	return 2 + ColorMode::NumberofParams();
}
String SinMode::GetName(int Number)
{
	auto lastno = ColorMode::NumberofParams();
	if (Number == 0 + lastno)
	{
		return "so";
	}
	else if (Number == 1 + lastno)
	{
		return "svo";
	}
	else
	{
		return ColorMode::GetName(Number);
	}
}
String SinMode::Get(String Name)
{
	if (Name == "so")
	{
		return String(HorizontalOffset);
	}
	else if (Name == "svo")
	{
		return String(VerticalOffset);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}