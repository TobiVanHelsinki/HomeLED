#pragma once

//Important build-settings
//#define HARDWARE_IS_ANALOG
#define HARDWARE_IS_NEOPIXEL
//#define _DEBUG

//Notes
//	For OTA : set FS to none


//Start of program
#ifdef _DEBUG
#define SERIALWRITELINE(x) Serial.println(x);
#define SERIALWRITE(x) Serial.print(x);
#else
#define SERIALWRITELINE(x)
#define SERIALWRITE(x)
#endif // _DEBUG

#include <Arduino.h>

//App Informaion
constexpr auto Manufactor = "Tobi van Helsinki, ImperiSoft";
constexpr auto ManufacturerURL = "https://github.com/Tobivanhelsinki/";
constexpr auto DeviceType = "ImperialHomeLED";
constexpr auto ModelURL = "https://github.com/Tobivanhelsinki/HomeLED";
constexpr auto Version = "1.8.4";
constexpr auto DEFAULTPASSW = "12345678";

//Technical Information
constexpr auto BuiltInLed = D4;
constexpr auto HTTPPort = 80;

//Settings - HW Reset
constexpr auto interruptPinReset = D7;
constexpr auto ResetPressedTime = 4000;


//Settings - Config
//constexpr auto StorageAdress_Start_Hostname = 0;
//constexpr auto StorageAdress_End_Hostname = 17;
//constexpr auto StorageAdress_Start_Configuration = 100;
//constexpr auto StorageAdress_End_Configuration = 500;
//constexpr auto StorageAdress_AutoConnect = 501;
//constexpr auto StorageAdress_EEPROMMax = 1024;
constexpr auto FileHostName = "/hostnm.txt";
constexpr auto FileConfig = "/lastconf.txt";
constexpr auto FileDatapin = "/datapin.txt";
constexpr auto FileLEDNo = "/ledno.txt";
