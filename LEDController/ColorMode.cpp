#include "ColorMode.h"

ColorMode::ColorMode(ILEDProvider* leds) : ModeBase(leds)
{
}

void ColorMode::NextState()
{
	for (uint16_t i = 0; i < leds->numPixels(); i++)
	{
		leds->setPixelColor(i, CurrentColor);
	}
}

String ColorMode::ID = "color";

String ColorMode::GetID()
{
	return ID;
}

std::vector<String> ColorMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("color");
	//names.push_back("color_h");
	//names.push_back("color_s");
	//names.push_back("color_v");
	auto baseNames = ModeBase::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String ColorMode::Get(String Name)
{
	if (Name == "color")
	{
		return String(CurrentColor);
	}
	//if (Name == "color_h")
	//{
	//	return String(CurrentColor_h);
	//}
	//else if (Name == "color_s")
	//{
	//	return String(CurrentColor_s);
	//}
	//else if (Name == "color_v")
	//{
	//	return String(CurrentColor_v);
	//}
	else
	{
		return ModeBase::Get(Name);
	}
}

String ColorMode::Set(String Name, String Value)
{
	if (Name == "color")
	{
		auto result = SetinBoundsAndReport(&CurrentColor, "Color", Value);
		RefreshColorParts();
		return result;
	}
	//else if (Name == "color_h")
	//{
	//	auto result = SetinBoundsAndReport(&CurrentColor_h, "Hue", Value);
	//	RefreshColor();
	//	return result;
	//}
	//else if (Name == "color_s")
	//{
	//	auto result = SetinBoundsAndReport(&CurrentColor_s, "Saturation", Value);
	//	RefreshColor();
	//	return result;
	//}
	//else if (Name == "color_v")
	//{
	//	auto result = SetinBoundsAndReport(&CurrentColor_v, "Value", Value);
	//	RefreshColor();
	//	return result;
	//}
	return ModeBase::Set(Name, Value);
}

void ColorMode::RefreshColorParts()
{
	//CurrentColor = Adafruit_NeoPixel::ColorHSV(CurrentColor_h, CurrentColor_s, CurrentColor_v);
	CurrentColor_r = CurrentColor >> 16;
	CurrentColor_g = CurrentColor >> 8;
	CurrentColor_b = CurrentColor >> 0;
	CalculateHSV(CurrentColor_r, CurrentColor_g, CurrentColor_b);
	if (DebugOutput)
	{
		Serial.print("r: ");
		Serial.println(CurrentColor_r);
		Serial.print("g: ");
		Serial.println(CurrentColor_g);
		Serial.print("b: ");
		Serial.println(CurrentColor_b);
		Serial.print("h: ");
		Serial.println(CurrentColor_h);
		Serial.print("s: ");
		Serial.println(CurrentColor_s);
		Serial.print("v: ");
		Serial.println(CurrentColor_v);
	}
}

void ColorMode::CalculateHSV(byte r, byte g, byte b)
{
	double rd = (double)r / 255;
	double gd = (double)g / 255;
	double bd = (double)b / 255;
	//Serial.print("p_r: ");
	//Serial.println(rd);
	//Serial.print("p_g: ");
	//Serial.println(gd);
	//Serial.print("p_b: ");
	//Serial.println(bd);

	double maxval = max(rd, max(gd, bd));
	double minval = min(rd, min(gd, bd));
	//Serial.print("max: ");
	//Serial.println(maxval);
	//Serial.print("min: ");
	//Serial.println(minval);
	double h, s, v = maxval;

	double d = maxval - minval;
	s = maxval == 0 ? 0 : d / maxval;

	if (maxval == minval) {
		h = 0; // achromatic
	}
	else {
		if (maxval == rd) {
			h = (gd - bd) / d + (gd < bd ? 6 : 0);
		}
		else if (maxval == gd) {
			h = (bd - rd) / d + 2;
		}
		else if (maxval == bd) {
			h = (rd - gd) / d + 4;
		}
		h /= 6;
	}
	//Serial.print("h: ");
	//Serial.println(h);
	//Serial.print("s: ");
	//Serial.println(s);
	//Serial.print("v: ");
	//Serial.println(v);

	CurrentColor_h = h * std::numeric_limits<uint16_t>::max();
	CurrentColor_s = s * std::numeric_limits<uint8_t>::max();
	CurrentColor_v = v * std::numeric_limits<uint8_t>::max();
}