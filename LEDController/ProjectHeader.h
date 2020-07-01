#pragma once

//#define CustomSettings
//#define HARDWARE_IS_ANALOG
#define HARDWARE_IS_NEOPIXEL
#define _DEBUG

#ifdef _DEBUG
#define SERIALWRITELINE(x) Serial.println(x);
#define SERIALWRITE(x) Serial.print(x);
#else
#define SERIALWRITELINE(x)
#define SERIALWRITE(x)
#endif // _DEBUG

/*
For OTA: set FS to none
*/
#include <Arduino.h>
constexpr auto BuiltInLed = D4;

//NetworkCommunication
constexpr auto Manufactor = "Tobi van Helsinki, ImperiSoft";
constexpr auto ManufacturerURL = "https://github.com/Tobivanhelsinki/";
constexpr auto DeviceType = "ImperialHomeLED";
constexpr auto ModelName = "ImperialHomeLED.V1";
constexpr auto ModelURL = "https://github.com/Tobivanhelsinki/HomeLED";
constexpr auto ModelNumber = "100";
constexpr auto Version = "1.7";
constexpr auto DEFAULTPASSW = "12345678";

constexpr auto HTTPPort = 80;

//HW Reset
constexpr auto interruptPinReset = D7;
constexpr auto ResetPressedTime = 4000;

constexpr auto HomeLEDTitle = "HomeLED-";

//Config
constexpr auto StorageAdress_Start_Hostname = 0;
constexpr auto StorageAdress_End_Hostname = 17;
constexpr auto StorageAdress_Start_Configuration = 100;
constexpr auto StorageAdress_End_Configuration = 500;
constexpr auto StorageAdress_AutoConnect = 501;
constexpr auto StorageAdress_EEPROMMax = 1024;

//LedFunctions
#ifdef HARDWARE_IS_NEOPIXEL
#ifdef CustomSettings
constexpr auto LEDsPin = D6;
#else
constexpr auto LEDsPin = D1;
#endif
#endif
#ifdef HARDWARE_IS_ANALOG
constexpr auto AnalogPin_R = D1;
constexpr auto AnalogPin_G = D3;
constexpr auto AnalogPin_B = D2;
#endif

constexpr auto MinNumberOfLeds = 1;
constexpr auto MinLEDRefreshTime = 20;
constexpr auto MinBrigthnes = 0;

#ifdef CustomSettings
constexpr auto MaxNumberOfLeds = 252;
#else
constexpr auto MaxNumberOfLeds = 300;
#endif

constexpr auto MaxLEDRefreshTime = 10000;
constexpr auto MaxBrigthnes = 255;

constexpr auto StartMode = "sin";