#pragma once
#include "TwoColorMode.h"
class TykeMode :
	public TwoColorMode
{
public:
	TykeMode(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();

protected:
	uint Position = 0;
};
