#pragma once
#include "ColorMode.h"
class KnightRiderMode :
	public ColorMode
{
public:
	KnightRiderMode(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
	std::vector<String> ParameterNames() override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
protected:
	int Direction = false;
	int PositionForward = 0;
	int PositionBackward = 0;
	int Width = 10;
	int Fringe = 0;
};
