#pragma once
#include "ILEDProvider.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class ModeBase
{
public:
	ModeBase(ILEDProvider* ledstouse);

	virtual String GetID() = 0;
	virtual void NextState() = 0;

	virtual std::vector<String> ParameterNames();
	virtual String Get(String Name);
	virtual String Set(String Name, String Value);

protected:
	ILEDProvider* leds;
	static String SetinBoundsAndReport(int* savePlace, String name, String parameter, int min = 0, int max = std::numeric_limits<int>::max());
	static String SetinBoundsAndReport(uint32_t* savePlace, String name, String parameter, uint32_t min = 0, uint32_t max = std::numeric_limits<uint32_t>::max());
	static String SetinBoundsAndReport(float* savePlace, String name, String parameter, float min = 0, float max = std::numeric_limits<float>::max());
	static String SetinBoundsAndReport(bool* savePlace, String name, String parameter);
	static uint32_t Wheel(byte WheelPos);
};
