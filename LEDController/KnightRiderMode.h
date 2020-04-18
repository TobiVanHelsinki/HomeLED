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
	void PrintTail(size_t pos);
	uint32_t CalculateFringedColor(size_t pos);
	size_t CalculateMirroredPosition(size_t pos);
	int CurrentDirection = false;
	int CurrentPosition = 0;
	int PositionBackward = 0;
	int Width = 10;
	int Fringe = 0;
};
