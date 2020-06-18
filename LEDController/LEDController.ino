#include "LEDController.h"

#pragma region Server Vars
bool IsServerReady = false;
ESP8266WebServer Server(HTTPPort);
AutoConnect Portal(Server);
AutoConnectUpdate UpdateService("home", 8010);
#pragma endregion

#pragma region Led Vars
ILEDProvider* leds =
#ifdef HARDWARE_IS_NEOPIXEL
new LEDProvider_NeoPixel(new Adafruit_NeoPixel(CurrentNumberOfLeds, LEDsPin, NEO_GRB + NEO_KHZ800));
#endif
#ifdef HARDWARE_IS_ANALOG
new LEDProvider_Analog(AnalogPin_R, AnalogPin_G, AnalogPin_B);
#endif
ModeBase* CurrentMode;
os_timer_t ShowTimer;
bool IsLEDStarted = false;
#pragma endregion

#pragma region Setups

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
	//ResetSystem();
	SetupResetProcedures();
	SetupLeds();
	SetupWiFi();
	SetupSSDP();
	SERIALWRITELINE("INIT complete");
}

void SetupResetProcedures()
{
	pinMode(interruptPinReset, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPinReset), ProcessResetInterrupt, CHANGE);
}

void SetupWiFi()
{
	SERIALWRITELINE("SetupWiFi");
	Server.on("/", handleRoot);
	Server.on("", handleRoot);
	AutoConnectConfig acConfig;
	acConfig.boundaryOffset = StorageAdress_AutoConnect;
	acConfig.title = HomeLEDTitle + String("MenuV") + Version;
	acConfig.apid = GenerateDefaultHostname();
	acConfig.psk = DEFAULTPASSW;
	//acConfig.autoReconnect = false;
	//acConfig.autoReset = false;
	//acConfig.autoRise = true;
	//acConfig.autoSave = AC_SAVECREDENTIAL_AUTO;
	acConfig.ota = AC_OTA_BUILTIN;
	acConfig.portalTimeout = 0; //0=endless
	acConfig.hostName = ReadValidHostname();
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
	SetMode("cp");
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
	auto name = ReadValidHostname();
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

void SetupLeds()
{
	SERIALWRITELINE("SetupLeds");
	leds->begin();
	leds->clear();

	leds->setPixelColor(1, Adafruit_NeoPixel::Color(20, 20, 255));
	leds->show();
	RestoreConfig();
	if (CurrentMode == NULL)
	{
		if (!SetMode(StartMode))
		{
			SERIALWRITELINE("\tError SetMode");
		}
	}
	SERIALWRITE("\t");
	LEDsStart();
}

#pragma endregion

void handleRoot()
{
	String result;
	for (auto i = 0; i < Server.args(); i++)
	{
		auto argName = Server.argName(i);
		auto argVal = Server.arg(i);
		if (argName == "get")
		{
			result += CurrentConfig2String();
		}
		else if (argName == "config")
		{
			if (argVal == "save")
			{
				result += StoreConfig();
			}
			else if (argVal == "load")
			{
				result += RestoreConfig();
			}
			else if (argVal == "clear")
			{
				result += ClearConfigMemory();
			}
		}
		else if (argName == "setHostname")
		{
			StoreHostname(argVal);
		}
		else
		{
			result += SetProperty(argName, argVal);
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

//Function is called by the timer multiple times a second
void RefreshLeds(void* pArg)
{
	CurrentMode->NextState();
	leds->show();
}
//TODO try to extract the following to other files
#pragma region Led Functs

void LEDsStart()
{
	if (IsLEDStarted)
	{
		return;
	}
	os_timer_setfn(&ShowTimer, RefreshLeds, NULL);
	os_timer_arm(&ShowTimer, CurrentLEDRefreshTime, true);
	SERIALWRITELINE("LEDs started");
	IsLEDStarted = true;
}

void LEDsStop()
{
	if (!IsLEDStarted)
	{
		return;
	}
	os_timer_disarm(&ShowTimer);
	delayMicroseconds(CurrentLEDRefreshTime + 20);
	SERIALWRITELINE("LEDs stopped");
	leds->clear();
	leds->show();
	IsLEDStarted = false;
}

bool SetMode(String s)
{
	auto tempMode = CurrentMode;
	if (s == "on")
	{
		LEDsStart();
		return true;
	}
	else if (s == "off")
	{
		LEDsStop();
		return true;
	}
	else if (CurrentMode != NULL && s == CurrentMode->GetID())
	{
		return false;
	}
	else if (s == "cp")
	{
		LEDsStop();
		return true;
		CurrentMode = new OnePixelMode(leds);
		CurrentMode->Set("CurrentColor", String(Adafruit_NeoPixel::Color(11, 200, 0)));
	}
	else if (s == RainbowMode::ID)
	{
		CurrentMode = new RainbowMode(leds);
	}
	else if (s == ColorMode::ID)
	{
		CurrentMode = new ColorMode(leds);
	}
	else if (s == SinMode::ID)
	{
		CurrentMode = new SinMode(leds);
	}
	else if (s == PulseMode::ID)
	{
		CurrentMode = new PulseMode(leds);
	}
	else if (s == OnePixelMode::ID)
	{
		CurrentMode = new OnePixelMode(leds);
	}
	else if (s == KnightRiderMode::ID)
	{
		CurrentMode = new KnightRiderMode(leds);
	}
	else if (s == TykeMode::ID)
	{
		CurrentMode = new TykeMode(leds);
	}
	else if (s == DoorsMode::ID)
	{
		int maxpin = D2 > D3 ? D2 : D3;
		maxpin = maxpin > D4 ? maxpin : D4;
		maxpin = maxpin > D5 ? maxpin : D5;
		int* PinDoorMap = new int[maxpin];
		for (auto i = 0; i < maxpin; i++)
		{
			PinDoorMap[i] = -1;
		}
		PinDoorMap[D2] = 0;
		PinDoorMap[D3] = 1;
		PinDoorMap[D4] = 2;
		PinDoorMap[D5] = 3;
		CurrentMode = new DoorsMode(leds, 4, PinDoorMap);
	}
	else
	{
		return false;
	}
	delete tempMode;
	return true;
}

static int CropAtBounds(int newVal, int minVal, int maxVal)
{
	if (newVal < minVal)
	{
		return minVal;
	}
	else if (newVal > maxVal)
	{
		return maxVal;
	}
	return newVal;
}

// Hz Value from 1 to 50
bool UpdateSpeed(int newValue)
{
	if (newValue == 0)
	{
		newValue = 1;
	}
	newValue = CropAtBounds(1000 / newValue, MinLEDRefreshTime, MaxLEDRefreshTime);
	if (newValue != CurrentLEDRefreshTime)
	{
		CurrentLEDRefreshTime = newValue;
		LEDsStop();
		LEDsStart();
		return true;
	}
	return false;
}

bool UpdateNumOfLeds(int newValue)
{
	newValue = CropAtBounds(newValue, MinNumberOfLeds, MaxNumberOfLeds);
	if (CurrentNumberOfLeds != newValue)
	{
		CurrentNumberOfLeds = newValue;
		leds->clear();
		leds->updateLength(CurrentNumberOfLeds);
		return true;
	}
	return false;
}

bool UpdateBri(int newValue)
{
	newValue = CropAtBounds(newValue, MinBrigthnes, MaxBrigthnes);
	if (CurrentBrigthnes != newValue)
	{
		CurrentBrigthnes = newValue;
		leds->setBrightness(newValue);
		return true;
	}
	return false;
}

#pragma endregion

#pragma region Config
String SetProperty(String argName, String argVal)
{
	if (argName.isEmpty())
	{
		return "Emtpy Arg";
	}
	if (argVal.isEmpty())
	{
		return "Emtpy ArgVal";
	}
	SERIALWRITELINE("SetProperty (" + argName + ")=(" + argVal + ")");
	String Return;
	if (argName == "br" || argName == "brightnes")
	{
		if (UpdateBri(argVal.toInt()))
		{
			Return += "Brigthnes changed to: " + String(CurrentBrigthnes) + " \n";
		}
	}
	else if (argName == "n" || argName == "number")
	{
		if (UpdateNumOfLeds(argVal.toInt()))
		{
			Return += "Number of active LEDs changed to: " + String(CurrentNumberOfLeds) + " \n";
		}
	}
	else if (argName == "v" || argName == "speed")
	{
		if (UpdateSpeed(argVal.toInt()))
		{
			Return += "Speed changed to: " + String(CurrentLEDRefreshTime) + " \n";
		}
	}
	else if (argName == "m" || argName == "mode")
	{
		if (SetMode(argVal))
		{
			Return += "Changed Mode to: " + CurrentMode->GetID() + "\n";
		}
	}
	else
	{
		if (CurrentMode)
		{
			Return += CurrentMode->Set(argName, argVal);
		}
	}
	return Return;
}

String CurrentConfig2String()
{
	String Return;
	Return += "br=" + String(CurrentBrigthnes) + "&";
	Return += "n=" + String(CurrentNumberOfLeds) + "&";
	Return += "v=" + String(CurrentLEDRefreshTime) + "&";
	Return += "m=" + String(CurrentMode->GetID()) + "&";
	auto names = CurrentMode->ParameterNames();
	for (size_t i = 0; i < names.size(); i++)
	{
		auto parname = names.at(i);
		Return += String(parname) + "=" + String(CurrentMode->Get(parname)) + "&";
	}
	return Return;
}

String String2CurrentConfig(String config)
{
	if (config.isEmpty())
	{
		return "config was Empty";
	}
	String param;
	int counter = 0;
	String result;
	do
	{
		param = GetValue(config, '&', counter);
		counter++;
		auto name = GetValue(param, '=', 0);
		auto value = GetValue(param, '=', 1);
		result += SetProperty(name, value);
	}
	while (param != "");
	return result;
}

String GetValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++)
	{
		if (data.charAt(i) == separator || i == maxIndex)
		{
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}
	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

#pragma endregion

#pragma region Config IO

String RestoreConfig()
{
	return String2CurrentConfig(ReadEEPROM(StorageAdress_Start_Configuration));
}

String StoreConfig()
{
	if (WriteEEPROM(StorageAdress_Start_Configuration, CurrentConfig2String(), StorageAdress_End_Configuration))
	{
		return "SUCCESS storing Config";
	}
	else
	{
		return "ERROR storing Config";
	}
}

String ClearConfigMemory()
{
	if (ClearEEPROM(StorageAdress_Start_Configuration, StorageAdress_End_Configuration))
	{
		return "SUCCESS cleaning Config";
	}
	else
	{
		return "ERROR cleaning Config";
	}
}

String GenerateDefaultHostname()
{
	return HomeLEDTitle + String(ESP.getChipId(), HEX);
}

String ReadValidHostname()
{
	auto hostname = ReadEEPROM(StorageAdress_Start_Hostname);
	if (hostname.isEmpty())
	{
		SERIALWRITELINE("Stored Hostname was empty");
		hostname = GenerateDefaultHostname();
	}
	return hostname;
}

String StoreHostname(String hostname)
{
	SERIALWRITE("storing new hostname:-");
	SERIALWRITE(hostname);
	SERIALWRITELINE("-");
	if (WriteEEPROM(StorageAdress_Start_Hostname, hostname, StorageAdress_End_Hostname))
	{
		//SetupSSDP();
		//SetupWiFi();
		SSDP.end();
		SSDP.setName(ReadValidHostname());
		SSDP.begin();
		SERIALWRITE("your new hostname is:-");
		SERIALWRITE(ReadValidHostname());
		SERIALWRITELINE("-");
		//acConfig.hostName = customHostname;
		return "SUCCESS storing Hostname, restarted SSDP Server";
	}
	else
	{
		return "ERROR storing Hostname";
	}
}
#pragma endregion

#pragma region HW Reset
unsigned long ResetInitiatedAt = 0;
void ProcessResetInterrupt()
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

void ResetSystem()
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

#pragma endregion