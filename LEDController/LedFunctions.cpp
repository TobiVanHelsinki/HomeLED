#include "LedFunctions.h"

int LedFunctions::CurrentNumberOfLeds = MaxNumberOfLeds;
int LedFunctions::CurrentLEDRefreshTime = 100; //in ms
int LedFunctions::CurrentBrigthnes = 25;

ILEDProvider* LedFunctions::leds = new
#ifdef HARDWARE_IS_NEOPIXEL
LEDProvider_NeoPixel(new Adafruit_NeoPixel(CurrentNumberOfLeds, LEDsPin, NEO_GRB + NEO_KHZ800))
#endif
#ifdef HARDWARE_IS_ANALOG
LEDProvider_Analog(AnalogPin_R, AnalogPin_G, AnalogPin_B)
#endif
;
ModeBase* LedFunctions::CurrentMode;
os_timer_t LedFunctions::ShowTimer;
bool LedFunctions::IsLEDStarted = false;

void LedFunctions::SetupLeds()
{
	SERIALWRITELINE("SetupLeds");
	leds->begin();
	//leds->clear();

	leds->fill(1,100, Adafruit_NeoPixel::Color(20, 20, 255));
	leds->show();
	//String2CurrentConfig(ReadEEPROM(StorageAdress_Start_Configuration));
	String2CurrentConfig(ReadFile(FileConfig));
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
	os_timer_arm(&ShowTimer, CurrentLEDRefreshTime, true);
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
	leds->clear();
	leds->show();
	IsLEDStarted = false;
}

bool LedFunctions::SetMode(String s)
{
	auto tempMode = CurrentMode;
	if (s == "on")
	{
		LEDsStart();
		return true;
	}
	else if (s == "off")
	{
		LEDsStop();
		return true;
	}
	else if (CurrentMode != NULL && s == CurrentMode->GetID())
	{
		return false;
	}
	else if (s == "cp")
	{
		LEDsStop();
		return true;
		CurrentMode = new OnePixelMode(leds);
		CurrentMode->Set("CurrentColor", String(Adafruit_NeoPixel::Color(11, 200, 0)));
	}
	else if (s == RainbowMode::ID)
	{
		CurrentMode = new RainbowMode(leds);
	}
	else if (s == ColorMode::ID)
	{
		CurrentMode = new ColorMode(leds);
	}
	else if (s == SinMode::ID)
	{
		CurrentMode = new SinMode(leds);
	}
	else if (s == PulseMode::ID)
	{
		CurrentMode = new PulseMode(leds);
	}
	else if (s == OnePixelMode::ID)
	{
		CurrentMode = new OnePixelMode(leds);
	}
	else if (s == KnightRiderMode::ID)
	{
		CurrentMode = new KnightRiderMode(leds);
	}
	else if (s == TykeMode::ID)
	{
		CurrentMode = new TykeMode(leds);
	}
	else if (s == DoorsMode::ID)
	{
		int maxpin = D2 > D3 ? D2 : D3;
		maxpin = maxpin > D4 ? maxpin : D4;
		maxpin = maxpin > D5 ? maxpin : D5;
		int* PinDoorMap = new int[maxpin];
		for (auto i = 0; i < maxpin; i++)
		{
			PinDoorMap[i] = -1;
		}
		PinDoorMap[D2] = 0;
		PinDoorMap[D3] = 1;
		PinDoorMap[D4] = 2;
		PinDoorMap[D5] = 3;
		CurrentMode = new DoorsMode(leds, 4, PinDoorMap);
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

bool LedFunctions::UpdateSpeed(int newValue)
{
	newValue = CropAtBounds(newValue, MinLEDRefreshTime, MaxLEDRefreshTime);
	if (newValue != CurrentLEDRefreshTime)
	{
		CurrentLEDRefreshTime = newValue;
		LEDsStop();
		LEDsStart();
		return true;
	}
	return false;
}

bool LedFunctions::UpdateNumOfLeds(int newValue)
{
	newValue = CropAtBounds(newValue, MinNumberOfLeds, MaxNumberOfLeds);
	if (CurrentNumberOfLeds != newValue)
	{
		CurrentNumberOfLeds = newValue;
		leds->clear();
		leds->updateLength(CurrentNumberOfLeds);
		return true;
	}
	return false;
}

bool LedFunctions::UpdateBri(int newValue)
{
	newValue = CropAtBounds(newValue, MinBrigthnes, MaxBrigthnes);
	if (CurrentBrigthnes != newValue)
	{
		CurrentBrigthnes = newValue;
		leds->setBrightness(newValue);
		return true;
	}
	return false;
}

String LedFunctions::SetProperty(String argName, String argVal)
{
	if (argName.isEmpty())
	{
		return "Emtpy Arg";
	}
	if (argVal.isEmpty())
	{
		return "Emtpy ArgVal";
	}
	SERIALWRITELINE("SetProperty (" + argName + ")=(" + argVal + ")");
	String Return;
	if (argName == "br" || argName == "brightnes")
	{
		if (UpdateBri(argVal.toInt()))
		{
			Return += "Brigthnes changed to: " + String(CurrentBrigthnes) + " \n";
		}
	}
	else if (argName == "n" || argName == "number")
	{
		if (UpdateNumOfLeds(argVal.toInt()))
		{
			Return += "Number of active LEDs changed to: " + String(CurrentNumberOfLeds) + " \n";
		}
	}
	else if (argName == "v" || argName == "speed")
	{
		if (UpdateSpeed(argVal.toInt()))
		{
			Return += "Speed changed to: " + String(CurrentLEDRefreshTime) + " \n";
		}
	}
	else if (argName == "m" || argName == "mode")
	{
		if (SetMode(argVal))
		{
			Return += "Changed Mode to: " + CurrentMode->GetID() + "\n";
		}
	}
	else
	{
		if (CurrentMode)
		{
			Return += CurrentMode->Set(argName, argVal);
		}
	}
	return Return;
}

String LedFunctions::CurrentConfig2String()
{
	String Return;
	Return += "br=" + String(CurrentBrigthnes) + "&";
	Return += "n=" + String(CurrentNumberOfLeds) + "&";
	Return += "v=" + String(CurrentLEDRefreshTime) + "&";
	Return += "m=" + String(CurrentMode->GetID()) + "&";
	auto names = CurrentMode->ParameterNames();
	for (size_t i = 0; i < names.size(); i++)
	{
		auto parname = names.at(i);
		Return += String(parname) + "=" + String(CurrentMode->Get(parname)) + "&";
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
		result += SetProperty(name, value);
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