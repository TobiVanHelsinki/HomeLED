#include "LedFunctions.h"

int LedFunctions::CurrentNumberOfLeds = MaxNumberOfLeds;
int LedFunctions::CurrentLEDRefreshTime = 100; //in ms
int LedFunctions::CurrentBrigthnes = 25;

#ifdef HARDWARE_IS_NEOPIXEL
#ifdef CustomSettings
int LedFunctions::LEDsPin = D1;
#else
int LedFunctions::LEDsPin = D1;
#endif 
#endif

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

	leds->fill(1,3, Adafruit_NeoPixel::Color(20, 20, 255));
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
	//If adding areas, here is the rigth place for the area-switch (a whileloop)
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
		CurrentMode = new ColorMode(leds);
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
		leds->fill(0, CurrentNumberOfLeds+1, 1023);
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

String LedFunctions::HandleProperty(String argName, String argVal)
{
	String result;
	if (argName == "b" || argName == "br" || argName == "brightnes")
	{
		if (!argVal.isEmpty())
		{
			UpdateBri(argVal.toInt());
		}
		result += "n=" + String(CurrentBrigthnes) + "&";
	}
	else if (argName == "n" || argName == "number")
	{
		//TODO Save like hostname
		if (!argVal.isEmpty())
		{
			UpdateNumOfLeds(argVal.toInt());
		}
		result += "n=" + String(CurrentNumberOfLeds) + "&";
	}
	else if (argName == "v" || argName == "speed")
	{
		if (!argVal.isEmpty())
		{
			UpdateSpeed(argVal.toInt());
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
			LEDsPin = argVal.toInt(); //TODO maybe boundry checks
			//TODO permanent store like hostname
		}
		result += "datapin=" + String(LEDsPin) + "&";
	}
	else
	{
		if (CurrentMode)
		{
			result += CurrentMode->HandleProperty(argName, argVal);
		}
	}
	return result;
}

String LedFunctions::CurrentConfig2String()
{
	String Return;
	Return += "b=" + String(CurrentBrigthnes) + "&";
	Return += "n=" + String(CurrentNumberOfLeds) + "&"; //TODO n hier entfernen
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