#pragma once
#include "ModeBase.h"
class RainbowMode :
	public ModeBase
{
public:
	RainbowMode(ILEDProvider* leds);

	void NextState();
	String ID();
	int NumberofParams() override;
	String GetName(int Number) override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
private:
	bool DisturbingMode = false;
	int j = 0;
};
