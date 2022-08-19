#pragma once
#include "ILEDProvider.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

class ModeBase
{
public:
	ModeBase(ILEDProvider* ledstouse);
	virtual ~ModeBase();

	virtual String GetID() = 0;
	virtual void NextState() = 0;

	virtual std::vector<String> ParameterNames();
	virtual String HandleProperty(String Name, String Value);

protected:
	//enable more debug msg over console
	bool DebugOutput = false;
	//the number of leds that act as one
	uint8_t StepSize = 1;
	//the number of leds, that should be ignored between steps
	uint8_t Skip = 0;
	//to control direction of any animation
	bool reverseDirection = false;
	//the base object for led-operations
	ILEDProvider* leds;
	static String SetinBoundsAndReport(int* savePlace, String name, String parameter, int min = 0, int max = std::numeric_limits<int>::max());
	static String SetinBoundsAndReport(uint32_t* savePlace, String name, String parameter, uint32_t min = 0, uint32_t max = std::numeric_limits<uint32_t>::max());
	static String SetinBoundsAndReport(uint16_t* savePlace, String name, String parameter, uint16_t min = 0, uint16_t max = std::numeric_limits<uint16_t>::max());
	static String SetinBoundsAndReport(uint8_t* savePlace, String name, String parameter, uint8_t min = 0, uint8_t max = std::numeric_limits<uint8_t>::max());
	static String SetinBoundsAndReport(float* savePlace, String name, String parameter, float min = 0, float max = std::numeric_limits<float>::max());
	static String SetinBoundsAndReport(bool* savePlace, String name, String parameter);
	static uint32_t Wheel(uint16_t WheelPos, byte White);
};
