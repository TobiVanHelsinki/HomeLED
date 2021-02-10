#include <Arduino.h>
#include "ProjectHeader.h"
#include "LedFunctions.h"
#include "NetworkCommunication.h"
#include "HWReset.h"
#include "ConfigIO.h"
#include "EEPROMHelper.h"
#include "ILEDProvider.h"

void setup()
{
#ifdef _DEBUG
	Serial.begin(115200);
#endif // _DEBUG
	delay(2000);
	SERIALWRITELINE("-------------- --------------");
	PrintResetCause();
	//InitEEPROM(StorageAdress_EEPROMMax);
	InitFileSystem();
	HWReset::SetupResetProcedures();
	//HWReset::ResetSystem();
	LedFunctions::SetupLeds();
	NetworkCommunication::SetupWiFi();
	NetworkCommunication::SetupSSDP();
	SERIALWRITELINE("INIT complete");
}

void loop(void)
{
	if (NetworkCommunication::IsServerReady)
	{
		NetworkCommunication::Portal.handleClient();
	}
	delay(1000);
}

void PrintResetCause()
{
	//https://www.espressif.com/sites/default/files/documentation/esp8266_reset_causes_and_common_fatal_exception_causes_en.pdf
	SERIALWRITELINE("Reset Cause");
	struct	rst_info* rtc_info = system_get_rst_info();
	SERIALWRITE("reset	reason:	");
	SERIALWRITELINE(rtc_info->reason);

	if (rtc_info->reason == REASON_WDT_RST ||
		rtc_info->reason == REASON_EXCEPTION_RST ||
		rtc_info->reason == REASON_SOFT_WDT_RST)
	{
		if (rtc_info->reason == REASON_EXCEPTION_RST)
		{
			SERIALWRITE("Fatal	exception: ");
			SERIALWRITELINE(rtc_info->exccause);
		}
		SERIALWRITE("epc1=");
		SERIALWRITELINE(rtc_info->epc1);
		SERIALWRITE("epc2=");
		SERIALWRITELINE(rtc_info->epc2);
		SERIALWRITE("epc3=");
		SERIALWRITELINE(rtc_info->epc3);
		SERIALWRITE("excvaddr=");
		SERIALWRITELINE(rtc_info->excvaddr);
		SERIALWRITE("depc=");
		SERIALWRITELINE(rtc_info->depc);
	}
}