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
#include "RainbowMode.h"
#include "PulseMode.h"
#include "Mode_Demo.h"
#include "SinMode.h"
#include "ColorMode.h"
#include "KnightRiderMode.h"
#include "TykeMode.h"

class LedFunctions
{
	static int CurrentNumberOfLeds;
	static int CurrentLEDRefreshTime; //in Hz
	static int CurrentBrigthnes;

	static os_timer_t ShowTimer;
	static bool IsLEDStarted;

public:
	static ILEDProvider* leds;
	static ModeBase* CurrentMode;

	static void SetupLeds();
	static void LEDsStart();

	static void LEDsStop();

	static bool SetMode(String s);

	static int CropAtBounds(int newVal, int minVal, int maxVal);
	// Hz Value from 1 to 50
	static bool UpdateSpeed(int newValue);

	static bool UpdateNumOfLeds(int newValue);

	static bool UpdateBri(int newValue);

	static String SetProperty(String argName, String argVal);

	static String CurrentConfig2String();

	static String String2CurrentConfig(String config);

	static String GetValue(String data, char separator, int index);
};
