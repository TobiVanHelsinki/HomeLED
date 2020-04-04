#pragma once
#include "SinMode.h"

class DoorsMode : public SinMode
{
public:
	DoorsMode(Adafruit_NeoPixel* leds, int pin, int* doorPinMap);
	void NextState();
	String ID();
	virtual void ICACHE_RAM_ATTR Interrupt(int Pin) override;
private:
	void SetDoorPixels(int Start, int End, int CurrentColor);
	volatile bool* DoorOpen;
	volatile int DoorCount;
	volatile int* PinDoorMap;

	int PixelStartDiff = 10;
	int PixelPerDoor = 30;
	int PixelBetweenDoors = 15;

};
