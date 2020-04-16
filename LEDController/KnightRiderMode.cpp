#include "KnightRiderMode.h"
KnightRiderMode::KnightRiderMode(ILEDProvider* leds) : ColorMode(leds)
{
}

void KnightRiderMode::NextState()
{
	if (Direction)
	{
		if (PositionForward < leds->numPixels())
		{
			PositionForward = ColorLeft;
			Direction = !Direction;
		}
		else
		{
			PositionForward++;
		}
		for (int k = PositionForward; k >= 0; k--) {
			if (k > PositionForward - ColorLeft) {
				leds->setPixelColor(k, CurrentColor);
			}
			else {
				leds->setPixelColor(k, 0);
			}
		}
	}
	else
	{
		if (PositionBackward >= 0)
		{
			PositionBackward = leds->numPixels() - ColorLeft - 1;
			Direction = !Direction;
		}
		else
		{
			PositionBackward--;
		}
		for (int k = PositionBackward; k < leds->numPixels(); k++) {
			if (k < PositionBackward + ColorLeft) {
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

String KnightRiderMode::ID()
{
	return "knightrider";
}

String KnightRiderMode::Set(String Name, String Value)
{
	if (Name == "width")
	{
		auto newval = Value.toInt();
		if (newval != ColorLeft)
		{
			ColorLeft = newval;
			return "Set Width to " + String(ColorLeft) + "\n";
		}
	}
	else if (Name == "fringe")
	{
		auto newval = Value.toInt();
		if (newval != ColorRight)
		{
			ColorRight = newval;
			return "Set Fringe to " + String(ColorRight) + "\n";
		}
	}
	return ColorMode::Set(Name, Value);
}
int KnightRiderMode::NumberofParams()
{
	return 2 + ColorMode::NumberofParams();
}
String KnightRiderMode::GetName(int Number)
{
	auto lastno = ColorMode::NumberofParams();
	if (Number == 0 + lastno)
	{
		return "width";
	}
	else if (Number == 1 + lastno)
	{
		return "fringe";
	}
	else
	{
		return ColorMode::GetName(Number);
	}
}
String KnightRiderMode::Get(String Name)
{
	if (Name == "width")
	{
		return String(ColorLeft);
	}
	else if (Name == "fringe")
	{
		return String(ColorRight);
	}
	else
	{
		return ColorMode::Get(Name);
	}
}