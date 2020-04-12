#pragma once
#include <AutoConnect.h>
#include <PageStream.h>
#include <PageBuilder.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266SSDP.h>
#include <AutoConnectCredential.h>

#include "EEPROMHelper.h"

#include "RainbowMode.h"
#include "OnePixelMode.h"
#include "SinMode.h"
#include "ColorMode.h"
#include "DoorsMode.h"

const auto LEDsPin = D1;
const auto BuiltInLed = D4;
const auto interruptPinReset = D7;

auto CurrentNumberOfLeds = 150;
auto CurrentLEDRefreshTime = 60;
auto CurrentBrigthnes = 100;
auto StartMode = "sin";

constexpr auto Manufactor = "Tobi van Helsinki, ImperiSoft";
constexpr auto ManufacturerURL = "https://github.com/Tobivanhelsinki/";
constexpr auto DeviceType = "ImperialHomeLED";
constexpr auto ModelName = "ImperialHomeLED.V1";
constexpr auto ModelURL = "https://github.com/Tobivanhelsinki/HomeLED";
constexpr auto ModelNumber = "100";

constexpr auto HTTPPort = 80;
const String HomeLEDTitle = "HomeLED-";
const String DefaultPassword = "moose";

const auto StorageAdress_Start_Hostname = 0;
const auto StorageAdress_End_Hostname = 17;
const auto StorageAdress_Start_Configuration = 100;
const auto StorageAdress_End_Configuration = 500;
const auto StorageAdress_AutoConnect = 3500;
const auto StorageAdress_EEPROMMax = 4096;

const auto ResetPressedTime = 4000;

void ICACHE_RAM_ATTR HandleResetInterrupt();