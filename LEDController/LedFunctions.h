#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "ProjectHeader.h"
#include "ILEDProvider.h"
#include "ModeBase.h"
#include "EEPROMHelper.h"
#include "LEDProvider_Analog.h"
#include "LEDProvider_NeoPixel.h"
#ifdef HARDWARE_IS_NEOPIXEL
#include "LEDProvider_NeoPixel.h"
#endif
#ifdef HARDWARE_IS_ANALOG
#include "LEDProvider_Analog.h"
#endif
//Modes:
#include "Mode_Rainbow.h"
#include "Mode_Pulse.h"
#include "Mode_Demo.h"
#include "Mode_Sin.h"
#include "Mode_Color.h"
#include "Mode_KnightRider.h"
#include "Mode_Tyke.h"

class LedFunctions
{

	static os_timer_t ShowTimer;
	static bool IsLEDStarted;

public:
	static int CurrentLEDRefreshTime; //in Hz
	static int CurrentBrigthnes;

	static ILEDProvider* leds;
	static ModeBase* CurrentMode;

	static void SetupLeds();
	static void LEDsStart();

	static void LEDsStop();

	static bool SetMode(String s);

	static int CropAtBounds(int newVal, int minVal, int maxVal);

	static String HandleProperty(String argName, String argVal);

	static String CurrentConfig2String();

	static String String2CurrentConfig(String config);

	static String GetValue(String data, char separator, int index);
};
