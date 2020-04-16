#include "DoorsMode.h"

String DoorsMode::GetID()
{
	return "doors";
}

DoorsMode::DoorsMode(ILEDProvider* leds, int doorcount, int* pinDoorMap) : SinMode(leds)
{
	DoorCount = doorcount;
	DoorOpen = new bool[DoorCount];
	for (size_t i = 0; i < DoorCount; i++)
	{
		DoorOpen[i] = true;
	}
	Scaling = 0.5;
	SinTabelSize = PixelPerDoor;
	BuildTable(true);
	PinDoorMap = pinDoorMap;
}

void DoorsMode::NextState()
{
	leds->clear();
	for (size_t i = 0; i < DoorCount; i++)
	{
		if (DoorOpen[i])
		{
			auto Start = PixelStartDiff + i * (PixelBetweenDoors + PixelPerDoor);
			SetDoorPixels(Start, Start + PixelPerDoor, CurrentColor);
		}
	}
}

void DoorsMode::SetDoorPixels(int Start, int End, int CurrentColor)
{
	for (int i = 0; i < SinTabelSize; i++)
	{
		float scale = SinTable[i];
		auto color = Adafruit_NeoPixel::Color(
			(int)(CurrentColor_r * scale),
			(int)(CurrentColor_g * scale),
			(int)(CurrentColor_b * scale)
		);
		leds->setPixelColor(i + Start, color);
	}
}

//void ICACHE_RAM_ATTR DoorsMode::Interrupt(int Pin)
//{
//	auto sensorValue = analogRead(Pin);
//	auto door = PinDoorMap[Pin];
//	Serial.println("Pin " + String(Pin) + "Door" + String(door) + "sensorValue " + String(sensorValue));
//	if (door >=0)
//	{
//		DoorOpen[door] = sensorValue == 1023; //== 1023
//	}
//	SinMode::Interrupt(Pin);
//}