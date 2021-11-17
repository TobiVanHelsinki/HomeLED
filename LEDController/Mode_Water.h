#pragma once
#include "Mode_TwoColor.h"
class Mode_Water :
	public TwoColorMode
{
public:
	Mode_Water(ILEDProvider* leds);
	virtual void NextState();
	static String ID;
	String GetID();
	std::vector<String> ParameterNames() override;
	String HandleProperty(String Name, String Value) override;
protected:
	String Areas = "0u;75l;110t;150d;250l;280b,300u;400l,430t;500d;600u";
};
