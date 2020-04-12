#include "ModeBase.h"

ModeBase::ModeBase(ILEDProvider* ledstouse)
{
	delay(1);
	leds = ledstouse;
}

String ModeBase::Set(String Name, String Value)
{
	return "";
}
int ModeBase::NumberofParams()
{
	return 0;
}
String ModeBase::GetName(int Number)
{
	return "";
}
String ModeBase::Get(String Name)
{
	return String();
}

//void ICACHE_RAM_ATTR ModeBase::Interrupt(int Pin)
//{
//}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ModeBase::Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}