#pragma once
#include <AutoConnect.h>
#include <PageStream.h>
#include <PageBuilder.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266SSDP.h>
#include <AutoConnectCredential.h>
#include <Arduino.h>
#include "ProjectHeader.h"
#include "LedFunctions.h"
#include "HWReset.h"
#include "ConfigIO.h"
#include "EEPROMHelper.h"
#include "ILEDProvider.h"

constexpr auto BuiltInLed = D4;

constexpr auto Manufactor = "Tobi van Helsinki, ImperiSoft";
constexpr auto ManufacturerURL = "https://github.com/Tobivanhelsinki/";
constexpr auto DeviceType = "ImperialHomeLED";
constexpr auto ModelName = "ImperialHomeLED.V1";
constexpr auto ModelURL = "https://github.com/Tobivanhelsinki/HomeLED";
constexpr auto ModelNumber = "100";
constexpr auto Version = "1.7";
constexpr auto DEFAULTPASSW = "12345678";

constexpr auto HTTPPort = 80;

void ICACHE_RAM_ATTR ProcessResetInterrupt();

void ResetSystem();
