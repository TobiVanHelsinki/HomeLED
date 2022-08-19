#pragma once
#include "ModeBase.h"
#include "math.h"
//#include <FastLED_NeoPixel.h>

class Mode_Fire :
	public ModeBase
{
public:
	Mode_Fire(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
	std::vector<String> ParameterNames() override;
	String HandleProperty(String Name, String Value) override;
protected:
	// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
	uint8_t cooling = 50;

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
	uint8_t probabilitySparking = 120;

};
