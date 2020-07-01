#pragma once

#if _DEBUG
#define SERIALWRITELINE(x) SERIALWRITELINE(x);
#define SERIALWRITE(x) SERIALWRITE(x);
#else
#define SERIALWRITELINE(x)
#define SERIALWRITE(x)
#endif // _DEBUG

/*
For OTA: set FS to none
*/
#include <Arduino.h>

constexpr auto interruptPinReset = D7;
constexpr auto ResetPressedTime = 4000;

constexpr auto HomeLEDTitle = "HomeLED-";

constexpr auto StorageAdress_Start_Hostname = 0;
constexpr auto StorageAdress_End_Hostname = 17;
constexpr auto StorageAdress_Start_Configuration = 100;
constexpr auto StorageAdress_End_Configuration = 500;
constexpr auto StorageAdress_AutoConnect = 501;
constexpr auto StorageAdress_EEPROMMax = 1024;