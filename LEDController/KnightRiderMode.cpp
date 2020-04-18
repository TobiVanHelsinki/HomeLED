#include "KnightRiderMode.h"
KnightRiderMode::KnightRiderMode(ILEDProvider* leds) : ColorMode(leds)
{
	CurrentColor = Adafruit_NeoPixel::Color(255, 0, 0);
}

void KnightRiderMode::NextState()
{
	//leds->clear();
	if (Direction)
	{
		if (PositionForward >= leds->numPixels())
		{
			PositionForward = Width;
			Direction = !Direction;
		}
		else
		{
			PositionForward++;
		}
		for (int k = PositionForward; k >= 0; k--) {
			if (k > PositionForward - Width) {
				leds->setPixelColor(k, CurrentColor);
			}
			else {
				leds->setPixelColor(k, 0);
			}
		}
	}
	else
	{
		if (PositionBackward < 0)
		{
			PositionBackward = leds->numPixels() - Width - 1;
			Direction = !Direction;
		}
		else
		{
			PositionBackward--;
		}
		for (int k = PositionBackward; k < leds->numPixels(); k++) {
			if (k < PositionBackward + Width) {
				leds->setPixelColor(k, CurrentColor);
			}
			else {
				leds->setPixelColor(k, 0);
			}
		}
	}

	//for (int PositionForward = Width; PositionForward < leds->numPixels(); PositionForward++) { // For each pixel in strip...
	//	for (int k = PositionForward; k >= 0; k--) {
	//		if (k > PositionForward - Width) {
	//			leds->setPixelColor(k, CurrentColor);
	//		}
	//		else {
	//			leds->setPixelColor(k, 0);
	//		}
	//	}
	//	leds->show();                          //  Update strip to match
	//	delay(wait);                           //  Pause for a moment
	//}
	//for (int PositionBackward = leds->numPixels() - Width - 1; PositionBackward >= 0; PositionBackward--) { // For each pixel in strip...
	//	for (int k = PositionBackward; k < leds->numPixels(); k++) {
	//		if (k < PositionBackward + Width) {
	//			leds->setPixelColor(k, CurrentColor);
	//		}
	//		else {
	//			leds->setPixelColor(k, 0);
	//		}
	//	}
	//	leds->show();                          //  Update strip to match
	//	delay(wait);                           //  Pause for a moment
	//}
}

String KnightRiderMode::ID = "knightrider";

String KnightRiderMode::GetID()
{
	return ID;
}

std::vector<String> KnightRiderMode::ParameterNames()
{
	std::vector<String> names;
	names.push_back("width");
	names.push_back("fringe");
	auto baseNames = ColorMode::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String KnightRiderMode::Get(String Name)
{
	if (Name == "width")
	{
		return String(Width);
	}
	else if (Name == "fringe")
	{
		return String(Fringe);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}

String KnightRiderMode::Set(String Name, String Value)
{
	if (Name == "width")
	{
		return SetinBoundsAndReport(&Width, "Width", Value);
	}
	else if (Name == "fringe")
	{
		return SetinBoundsAndReport(&Fringe, "Fringe", Value);
	}
	return ColorMode::Set(Name, Value);
}