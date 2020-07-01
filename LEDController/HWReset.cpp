#include "HWReset.h"
unsigned long HWReset::ResetInitiatedAt = 0;

void HWReset::SetupResetProcedures()
{
	pinMode(interruptPinReset, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPinReset), HWReset::ProcessResetInterrupt, CHANGE);
}

void ICACHE_RAM_ATTR HWReset::ProcessResetInterrupt()
{
	if (digitalRead(interruptPinReset) == HIGH) //released
	{
		auto pressedTime = millis() - ResetInitiatedAt;
		if (pressedTime > ResetPressedTime)
		{
			SERIALWRITE("Reset Button released after time.");
			ResetSystem();
		}
		else
		{
			SERIALWRITELINE("Abort Reset. (" + String(pressedTime) + ") milliseconds pressed");
		}
	}
	else //pressed
	{
		SERIALWRITELINE("Reset Button pressed. Releasing it in more then 4 sec will reset the module.");
		ResetInitiatedAt = millis();
	}
}

void HWReset::ResetSystem()
{
	SERIALWRITELINE("Reset and cleaning memory now!");
	//ClearConfigMemory();
	ClearEEPROM(0, StorageAdress_EEPROMMax); //geht auch nicht
											 //auto credential = AutoConnectCredential(0);
											 //AutoConnect::;
	SERIALWRITELINE("Remove AutoConnectCredential now!");
	AutoConnectCredential credential;
	station_config_t config;
	uint8_t ent = credential.entries();
	SERIALWRITE("Count of stored credentials: ");
	SERIALWRITELINE(ent); //TODO geht noch nicht. gibt 0 zurück
						 //https://hieromon.github.io/AutoConnect/credit.html#constructors
	while (ent > 0)
	{
		credential.load((int8_t)0, &config);
		SERIALWRITE("Clearing WiFi Credentials for:");
		SERIALWRITELINE((const char*)&config.ssid[0]);
		credential.del((const char*)&config.ssid[0]);
		ent--;
	}
	ESP.restart(); // macht millis() unbrauchbar
}