#include "ColorMode.h"

ColorMode::ColorMode(ILEDProvider* leds) : ModeBase(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(229, 130, 00);
	RefreshCurrentColors();
}

void ColorMode::NextState()
{
	for (uint16_t i = 0; i < leds->numPixels(); i++)
	{
		leds->setPixelColor(i, CurrentColor);
	}
}

String ColorMode::ID = "c";

String ColorMode::GetID()
{
	return ID;
}

std::vector<String> ColorMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("c");
	auto baseNames = ModeBase::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String ColorMode::Get(String Name)
{
	if (Name == "c")
	{
		return String(CurrentColor);
	}
	else
	{
		return ModeBase::Get(Name);
	}
}

String ColorMode::Set(String Name, String Value)
{
	if (Name == "c")
	{
		auto result = SetinBoundsAndReport(&CurrentColor, "c", Value);
		RefreshCurrentColors();
		if (DebugOutput)
		{
			SERIALWRITE("r: ");
			SERIALWRITELINE(CurrentColor_r);
			SERIALWRITE("g: ");
			SERIALWRITELINE(CurrentColor_g);
			SERIALWRITE("b: ");
			SERIALWRITELINE(CurrentColor_b);
			SERIALWRITE("h: ");
			SERIALWRITELINE(CurrentColor_h);
			SERIALWRITE("s: ");
			SERIALWRITELINE(CurrentColor_s);
			SERIALWRITE("v: ");
			SERIALWRITELINE(CurrentColor_v);
		}
		return result;
	}
	else
	{
		return ModeBase::Set(Name, Value);
	}
}

void ColorMode::RefreshCurrentColors()
{
	CalculateRGB(CurrentColor, &CurrentColor_r, &CurrentColor_g, &CurrentColor_b);
	CalculateHSV(CurrentColor_r, CurrentColor_g, CurrentColor_b, &CurrentColor_h, &CurrentColor_s, &CurrentColor_v);
}

void ColorMode::CalculateRGB(uint32_t color, uint8_t* colorr, uint8_t* colorg, uint8_t* colorb)
{
	*(colorr) = color >> 16;
	*(colorg) = color >> 8;
	*(colorb) = color >> 0;
}

void ColorMode::CalculateHSV(byte r, byte g, byte b, uint16_t* colorh, uint8_t* colors, uint8_t* colorv)
{
	double rd = (double)r / 255;
	double gd = (double)g / 255;
	double bd = (double)b / 255;

	double maxval = max(rd, max(gd, bd));
	double minval = min(rd, min(gd, bd));
	double h = 0, s, v = maxval;

	double d = maxval - minval;
	s = maxval == 0 ? 0 : d / maxval;

	if (maxval == minval)
	{
		h = 0; // achromatic
	}
	else
	{
		if (maxval == rd)
		{
			h = (gd - bd) / d + (gd < bd ? 6 : 0);
		}
		else if (maxval == gd)
		{
			h = (bd - rd) / d + 2;
		}
		else if (maxval == bd)
		{
			h = (rd - gd) / d + 4;
		}
		h /= 6;
	}

	*(colorh) = h * std::numeric_limits<uint16_t>::max();
	*(colors) = s * std::numeric_limits<uint8_t>::max();
	*(colorv) = v * std::numeric_limits<uint8_t>::max();
	//CurrentColor_h = h * std::numeric_limits<uint16_t>::max();
	//CurrentColor_s = s * std::numeric_limits<uint8_t>::max();
	//CurrentColor_v = v * std::numeric_limits<uint8_t>::max();
}