#pragma once
#include "ColorMode.h"
class KnightRiderMode :
	public ColorMode
{
public:
	KnightRiderMode(ILEDProvider* leds);
	virtual void NextState();
	String ID();
	int NumberofParams() override;
	String GetName(int Number) override;
	String Get(String Name) override;
	String Set(String Name, String Value) override;
protected:
	int Direction = false;
	int PositionForward = 0;
	int PositionBackward = 0;
	int ColorLeft = 10;
	int ColorRight = 0;
};
