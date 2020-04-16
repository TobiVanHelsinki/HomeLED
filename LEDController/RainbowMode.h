#pragma once
#include "ModeBase.h"

constexpr auto id = "rainbow";
class RainbowMode :
	public ModeBase
{
public:
	const static String ID;
	RainbowMode(ILEDProvider* leds);
	void NextState();
	String GetID();
	int NumberofParams() override;
	String GetName(int Number) override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
private:
	bool DisturbingMode = false;
	int j = 0;
};
