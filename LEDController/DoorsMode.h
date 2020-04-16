#pragma once
#include "SinMode.h"

class DoorsMode : public SinMode
{
public:
	DoorsMode(ILEDProvider* leds, int pin, int* doorPinMap);
	void NextState();
	static String ID;
	String GetID();
	//virtual void ICACHE_RAM_ATTR Interrupt(int Pin) override;
private:
	void SetDoorPixels(int Start, int End, int CurrentColor);
	volatile bool* DoorOpen;
	volatile int DoorCount;
	volatile int* PinDoorMap;

	int PixelStartDiff = 10;
	int PixelPerDoor = 30;
	int PixelBetweenDoors = 15;
};
