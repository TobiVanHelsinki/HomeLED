#pragma once
#include "ModeBase.h"

constexpr auto id = "rainbow";
class RainbowMode :
	public ModeBase
{
public:
	RainbowMode(ILEDProvider* leds);
	void NextState();
	static String ID;
	String GetID();
	std::vector<String> ParameterNames() override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
private:
	bool DisturbingMode = false;
	int j = 0;
};
