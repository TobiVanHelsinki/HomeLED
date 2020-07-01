#include "LEDController.h"

bool IsServerReady = false;
ESP8266WebServer Server(HTTPPort);
AutoConnect Portal(Server);
AutoConnectUpdate UpdateService("home", 8010);

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

void setup()
{
#if _DEBUG
	Serial.begin(115200);
#endif // _DEBUG

	delayMicroseconds(500);
	SERIALWRITELINE("-------------- --------------");
	PrintResetCause();
	InitEEPROM(StorageAdress_EEPROMMax);
	//HWReset::ResetSystem();
	SetupResetProcedures();
	LedFunctions::SetupLeds();
	SetupWiFi();
	SetupSSDP();
	SERIALWRITELINE("INIT complete");
}

void SetupResetProcedures()
{
	pinMode(interruptPinReset, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPinReset), HWReset::ProcessResetInterrupt, CHANGE);
}

void SetupWiFi()
{
	SERIALWRITELINE("SetupWiFi");
	Server.on("/", handleRoot);
	Server.on("", handleRoot);
	AutoConnectConfig acConfig;
	acConfig.boundaryOffset = StorageAdress_AutoConnect;
	acConfig.title = HomeLEDTitle + String("MenuV") + Version;
	acConfig.apid = ConfigIO::GenerateDefaultHostname();
	acConfig.psk = DEFAULTPASSW;
	//acConfig.autoReconnect = false;
	//acConfig.autoReset = false;
	//acConfig.autoRise = true;
	//acConfig.autoSave = AC_SAVECREDENTIAL_AUTO;
	acConfig.ota = AC_OTA_BUILTIN;
	acConfig.portalTimeout = 0; //0=endless
	acConfig.hostName = ConfigIO::ReadValidHostname();
	acConfig.apip = IPAddress(192, 168, 10, 1);
	acConfig.ticker = true;
	Portal.config(acConfig);
	Portal.onDetect(startCP);
	Portal.onNotFound(portalNotFound);
	SERIALWRITELINE("\tPortal.begin");
	IsServerReady = Portal.begin();
	if (IsServerReady)
	{
		//UpdateService.atta//(Portal);
		UpdateService.onStart(onUpdateStart);
		UpdateService.onEnd(onUpdateEnd);
		UpdateService.onProgress(onUpdateProgress);
		UpdateService.onError(onUpdateError);
		UpdateService.rebootOnUpdate(true);
		SERIALWRITELINE("\tWiFi connected");
		SERIALWRITE("\tSSID: ");
		SERIALWRITELINE(WiFi.SSID());
		SERIALWRITE("\tIP address: ");
		SERIALWRITELINE(WiFi.localIP());
		SERIALWRITE("\tHostname: ");
		SERIALWRITELINE(wifi_station_get_hostname());
		SERIALWRITELINE("\tWebServer Started");
	}
	else
	{
		SERIALWRITELINE("WebServer NOT ready, unknown error.");
	}
}

void portalNotFound()
{
	SERIALWRITELINE("portal: Not found");
}

bool startCP(IPAddress ip)
{
	LedFunctions::SetMode("cp");
	SERIALWRITELINE("CP (own WLAN) started, IP:" + WiFi.localIP().toString());
	return true;
}
void onUpdateStart()
{
	SERIALWRITELINE("Updateprocess started");
}
void onUpdateEnd()
{
	SERIALWRITELINE("Updateprocess ended");
}
void onUpdateProgress(int p, int p1)
{
	SERIALWRITELINE("Update makes progress:" + String(p) + String(p1));
}
void onUpdateError(int code)
{
	SERIALWRITELINE("Update encountered Error:" + String(code));
}

void SetupSSDP()
{
	SERIALWRITELINE("SetupSSDP");
	//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266SSDP/examples/SSDP/SSDP.ino
	Server.on("/index.html", HTTP_GET, []() { Server.send(200, "text/plain", "Hello World!"); });
	Server.on("/description.xml", HTTP_GET, []() { SERIALWRITELINE("\tSSDP Request"); SSDP.schema(Server.client()); });
	SSDP.setURL("index.html");
	SSDP.setSchemaURL("description.xml");
	SSDP.setHTTPPort(HTTPPort);
	SSDP.setManufacturer(Manufactor);
	SSDP.setManufacturerURL(ManufacturerURL);
	SSDP.setDeviceType(DeviceType);
	SSDP.setModelName(ModelName);
	SSDP.setModelURL(ModelURL);
	SSDP.setModelNumber(ModelNumber);
	auto serialNo = ModelNumber + String("_") + String(ESP.getChipId(), HEX);
	SSDP.setSerialNumber(serialNo);
	auto name = ConfigIO::ReadValidHostname();
	SSDP.setName(name);
	SERIALWRITE("\t");
	SERIALWRITE("Model: ");
	SERIALWRITELINE(ModelName);
	SERIALWRITE("Serial: ");
	SERIALWRITELINE(serialNo);
	SERIALWRITE("Name: ");
	SERIALWRITELINE(name);
	if (SSDP.begin())
	{
		SERIALWRITELINE("\tSSDP started");
	}
	else
	{
		SERIALWRITELINE("\tERROR starting SSDP");
	}
}

void handleRoot()
{
	String result;
	for (auto i = 0; i < Server.args(); i++)
	{
		auto argName = Server.argName(i);
		auto argVal = Server.arg(i);
		if (argName == "get")
		{
			result += LedFunctions::CurrentConfig2String();
		}
		else if (argName == "config")
		{
			if (argVal == "save")
			{
				result += ConfigIO::StoreConfig(LedFunctions::CurrentConfig2String());
			}
			else if (argVal == "load")
			{
				result += LedFunctions::String2CurrentConfig(ReadEEPROM(StorageAdress_Start_Configuration));
			}
			else if (argVal == "clear")
			{
				result += ConfigIO::ClearConfigMemory();
			}
		}
		else if (argName == "setHostname")
		{
			ConfigIO::StoreHostname(argVal);
			SSDP.end();
			SSDP.setName(ConfigIO::ReadValidHostname());
			SSDP.begin();
		}
		else
		{
			result += LedFunctions::SetProperty(argName, argVal);
		}
	}
	if (Server.args() == 0)
	{
		digitalWrite(BuiltInLed, LOW); //Led port einschlaten
		result = "Welcome to the server";
		delay(100);
		digitalWrite(BuiltInLed, HIGH); //Led port ausschalten
	}
	Server.send(200, "text/plain", result);
	SERIALWRITELINE(result);
}

void loop(void)
{
	if (IsServerReady)
	{
		Portal.handleClient();
	}
	delay(100);
}