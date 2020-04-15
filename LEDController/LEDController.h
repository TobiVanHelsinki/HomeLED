#pragma once

#define HARDWARE_IS_ANALOG
//#define HARDWARE_IS_NEOPIXEL

#include <AutoConnect.h>
#include <PageStream.h>
#include <PageBuilder.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266SSDP.h>
#include <AutoConnectCredential.h>
#include <pins_arduino.h>

#include "EEPROMHelper.h"
#include "ILEDProvider.h"
#ifdef HARDWARE_IS_NEOPIXEL
#include "LEDProvider_NeoPixel.h"
//constexpr auto LEDsPin = D1; //Tobi
constexpr auto LEDsPin = D6; //Tyke
#endif
#ifdef HARDWARE_IS_ANALOG
#include "LEDProvider_Analog.h"
constexpr auto AnalogPin_R = D1;
constexpr auto AnalogPin_G = D3;
constexpr auto AnalogPin_B = D2;
#endif

constexpr auto BuiltInLed = D4;
constexpr auto interruptPinReset = D7;

#include "RainbowMode.h"
#include "PulseMode.h"
#include "OnePixelMode.h"
#include "SinMode.h"
#include "ColorMode.h"
#include "DoorsMode.h"

constexpr auto MinNumberOfLeds = 1;
constexpr auto MinLEDRefreshTime = 20;
constexpr auto MinBrigthnes = 0;

constexpr auto MaxNumberOfLeds = 252; //Tyke
//constexpr auto MaxNumberOfLeds = 150; //Tobi
constexpr auto MaxLEDRefreshTime = 10000;
constexpr auto MaxBrigthnes = 255;

auto CurrentNumberOfLeds = MaxNumberOfLeds;
auto CurrentLEDRefreshTime = 60;
auto CurrentBrigthnes = 100;

constexpr auto StartMode = "sin";

constexpr auto Manufactor = "Tobi van Helsinki, ImperiSoft";
constexpr auto ManufacturerURL = "https://github.com/Tobivanhelsinki/";
constexpr auto DeviceType = "ImperialHomeLED";
constexpr auto ModelName = "ImperialHomeLED.V1";
constexpr auto ModelURL = "https://github.com/Tobivanhelsinki/HomeLED";
constexpr auto ModelNumber = "100";

constexpr auto HTTPPort = 80;
constexpr auto HomeLEDTitle = "HomeLED-";

constexpr auto StorageAdress_Start_Hostname = 0;
constexpr auto StorageAdress_End_Hostname = 17;
//constexpr auto StorageAdress_Start_Configuration = 100; //Tobi
//constexpr auto StorageAdress_End_Configuration = 500;
//constexpr auto StorageAdress_AutoConnect = 3500;
//constexpr auto StorageAdress_EEPROMMax = 4096;
constexpr auto StorageAdress_Start_Configuration = 50; //Tyke
constexpr auto StorageAdress_End_Configuration = 250;
constexpr auto StorageAdress_AutoConnect = 3500;
constexpr auto StorageAdress_EEPROMMax = 512;

constexpr auto ResetPressedTime = 4000;

void ICACHE_RAM_ATTR ProcessResetInterrupt();

void ResetSystem();
