#include "LedFunctions.h"

int LedFunctions::CurrentLEDRefreshTime = 100; //in ms
int LedFunctions::CurrentBrigthnes = 25;

ILEDProvider* LedFunctions::leds;
ModeBase* LedFunctions::CurrentMode;
os_timer_t LedFunctions::ShowTimer;
bool LedFunctions::IsLEDStarted = false;

void LedFunctions::SetupLeds()
{
	SERIALWRITELINE("SetupLeds");
	auto ledno = ReadFile(FileLEDNo).toInt();
	auto pin = ReadFile(FileDatapin).toInt(); //TODO auch f�r r,g,b einf�hren
	//TODO better sanitychecks
	switch (pin)
	{
	case 0:
		pin = D0; // reserved for intern-LED I thought
		break;
	case 1:
		pin = D1; //5-tb
		break;
	case 2:
		pin = D2; 
		break;
	case 3:
		pin = D3; 
		break;
	case 4:
		pin = D4;
		break;
	case 5:
		pin = D5;
		break;
	case 6:
		pin = D6; //12-tk
		break;
	case 7:
		pin = D7;
		break;
	case 8:
		pin = D8;
		break;
	//case 9:
	//	pin = D9;
	//	break;
	//case 10:
	//	pin = D10;
	//	break;
	default:
		pin = pin;
		break;
	}
	auto ledtype = ReadFile(FileLEDType).toInt(); //TODO auch f�r r,g,b einf�hren
	if (leds)
	{
		LEDsStop();
		delete(leds);
	}
#ifdef HARDWARE_IS_NEOPIXEL
	SERIALWRITELINE("Start NeoPixel at pin " + String(pin) +  ", number of leds: " + String(ledno));
	leds = new LEDProvider_NeoPixel(new Adafruit_NeoPixel(ledno, pin, ledtype + NEO_KHZ800));
#endif
#ifdef HARDWARE_IS_ANALOG
	leds = new LEDProvider_Analog(AnalogPin_R, AnalogPin_G, AnalogPin_B);
#endif
	leds->begin();
	leds->fill(1,3, Adafruit_NeoPixel::Color(20, 20, 255));
	leds->show();
	String2CurrentConfig(ReadFile(FileLastConfig));
	leds->setBrightness(255);

	if (CurrentMode == NULL)
	{
		if (!SetMode(StartMode))
		{
			SERIALWRITELINE("\tError SetMode");
		}
	}
	LedFunctions::LEDsStart();
}

//Function is called by the timer multiple times a second
LOCAL void ICACHE_FLASH_ATTR RefreshLeds(void* pArg) //LOCAL und FLASH ist neu
{
	LedFunctions::CurrentMode->NextState();
	LedFunctions::leds->show();
}

void LedFunctions::LEDsStart()
{
	if (IsLEDStarted)
	{
		return;
	}
	os_timer_disarm(&ShowTimer);
	os_timer_setfn(&ShowTimer, (os_timer_func_t*)RefreshLeds, (void*)0);
	if (CurrentLEDRefreshTime != 0)
	{
		os_timer_arm(&ShowTimer, CurrentLEDRefreshTime, true);
	}
	IsLEDStarted = true;
	SERIALWRITELINE("LEDs started");
}

void LedFunctions::LEDsStop()
{
	if (!IsLEDStarted)
	{
		return;
	}
	os_timer_disarm(&ShowTimer);
	delayMicroseconds(CurrentLEDRefreshTime + 20);
	SERIALWRITELINE("LEDs stopped");
	IsLEDStarted = false;
}

bool LedFunctions::SetMode(String s)
{
	auto tempMode = CurrentMode;
	if (s == "off")
	{
		LEDsStop();
		leds->clear();
		leds->show();
		return true;
	}
	if (s == "on")
	{
		LEDsStart();
		return true;
	}
	else if (CurrentMode != NULL && s == CurrentMode->GetID())
	{
		return false;
	}
	else if (s == "cp") //what is this?
	{
		LEDsStop();
		return true;
	}
	else if (s == RainbowMode::ID)
	{
		CurrentMode = new RainbowMode(leds);
	}
	else if (s == ColorMode::ID)
	{
		LEDsStop();
		CurrentMode = new ColorMode(leds);
		HandleProperty("v", "0"); // TODO Test -> geht, aver muss noch verfeinert werden. Am besten 0 zur�cksenden, sodass die UI dann nicht auf den gedanken kommt und wieder 40 sendet.
		LEDsStart();
	}
	else if (s == TwoColorMode::ID)
	{
		CurrentMode = new TwoColorMode(leds);
	}
	else if (s == SinMode::ID)
	{
		CurrentMode = new SinMode(leds);
	}
	else if (s == PulseMode::ID)
	{
		CurrentMode = new PulseMode(leds);
	}
	else if (s == Mode_Demo::ID)
	{
		CurrentMode = new Mode_Demo(leds);
	}
	else if (s == KnightRiderMode::ID)
	{
		CurrentMode = new KnightRiderMode(leds);
	}
	else if (s == TykeMode::ID)
	{
		CurrentMode = new TykeMode(leds);
	}
	else
	{
		return false;
	}
	delete tempMode;
	return true;
}

int LedFunctions::CropAtBounds(int newVal, int minVal, int maxVal)
{
	if (newVal < minVal)
	{
		return minVal;
	}
	else if (newVal > maxVal)
	{
		return maxVal;
	}
	return newVal;
}

String LedFunctions::HandleProperty(String argName, String argVal)
{
	bool reSetupLEDs = false;
	String result;
	if (argName == "n" || argName == "number")
	{
		if (!argVal.isEmpty())
		{
			auto newValue = CropAtBounds(argVal.toInt(), MinNumberOfLeds, MaxNumberOfLeds);
			if (ReadFile(FileLEDNo).compareTo(argVal) != 0)
			{
				if (WriteFile(FileLEDNo, argVal))
				{
					result += "SUCCESS storing n&";

				}
				else
				{
					result += "ERROR storing n&";
				}
				leds->fill(0, newValue, 1023);
				leds->show();
				leds->updateLength(newValue);
			}
		}
		result += "n=" + ReadFile(FileLEDNo) + "&";
	}
	else if (argName == "v" || argName == "speed")
	{
		if (!argVal.isEmpty())
		{
			auto newValue = CropAtBounds(argVal.toInt(), 0, MaxLEDRefreshTime);
			if (newValue != CurrentLEDRefreshTime) 
			{
				SERIALWRITELINE(newValue);
				CurrentLEDRefreshTime = newValue;
				LEDsStop();
				LEDsStart();
			}
		}
		result += "v=" + String(CurrentLEDRefreshTime) + "&";
	}
	else if (argName == "m" || argName == "mode")
	{
		if (!argVal.isEmpty())
		{
			SetMode(argVal);
		}
		result += "m=" + String(CurrentMode->GetID()) + "&";
	}
	else if (argName == "datapin")
	{
		if (!argVal.isEmpty())
		{
			auto newValue = argVal.toInt(); //TODO boundry checks, just allow some pins, use also at the top
			if (ReadFile(FileDatapin).compareTo(argVal) != 0)
			{
				if (WriteFile(FileDatapin, argVal))
				{
					result += "SUCCESS storing datapin&";
					reSetupLEDs = true;
				}
				else
				{
					result += "ERROR storing datapin&";
				}
			}
		}
		result += "datapin=" + ReadFile(FileDatapin) + "&";
	}
	else if (argName == "ledtype")
	{
		if (!argVal.isEmpty())
		{
			int newValue = 0;
			if (argVal.compareTo("NEO_GRBW") == 0)
			{
				newValue = NEO_GRBW;
			}
			else if (argVal.compareTo("NEO_GRB") == 0)
			{
				newValue = NEO_GRB;
			} 
			else
			{
				newValue = argVal.toInt();
			}
			auto newValueString = String(newValue);
			if (ReadFile(FileLEDType).compareTo(newValueString) != 0)
			{
				if (WriteFile(FileLEDType, newValueString))
				{
					result += "SUCCESS storing LED Type&";
					reSetupLEDs = true;
				}
				else
				{
					result += "ERROR storing LED Type&";
				}
			}
		}
		result += "ledtype=" + ReadFile(FileLEDType) + "&";
	}
	else
	{
		if (CurrentMode)
		{
			result += CurrentMode->HandleProperty(argName, argVal);
		}
	}
	if (reSetupLEDs)
	{
		SetupLeds();
	}
	return result;
}

String LedFunctions::CurrentConfig2String()
{
	String Return; 
	Return += "b=" + String(CurrentBrigthnes) + "&";
	Return += "v=" + String(CurrentLEDRefreshTime) + "&";
	Return += "m=" + String(CurrentMode->GetID()) + "&";
	auto names = CurrentMode->ParameterNames();
	for (size_t i = 0; i < names.size(); i++)
	{
		Return += CurrentMode->HandleProperty(names.at(i), "");
	}
	return Return;
}

String LedFunctions::String2CurrentConfig(String config)
{
	
	if (config.isEmpty())
	{
		SERIALWRITELINE("config was Empty");
		return "config was Empty";
	}
	SERIALWRITE("Load: ");
	SERIALWRITELINE(config);
	String param;
	int counter = 0;
	String result;
	do
	{
		param = GetValue(config, '&', counter);
		counter++;
		auto name = GetValue(param, '=', 0);
		auto value = GetValue(param, '=', 1);
		result += HandleProperty(name, value);
	}
	while (param != "");
	return result;
}

String LedFunctions::GetValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++)
	{
		if (data.charAt(i) == separator || i == maxIndex)
		{
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}
	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}