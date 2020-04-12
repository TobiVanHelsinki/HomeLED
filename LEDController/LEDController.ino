#include "LEDController.h"

#pragma region Server Vars
bool IsServerReady = false;
ESP8266WebServer Server(HTTPPort);
AutoConnect Portal(Server);
#pragma endregion

#pragma region Led Vars
Adafruit_NeoPixel leds = Adafruit_NeoPixel(CurrentNumberOfLeds, LEDsPin, NEO_GRB + NEO_KHZ800);
ModeBase* CurrentMode;
os_timer_t ShowTimer;
bool LEDsStarted = false;
#pragma endregion

#pragma region Setups

void setup()
{
	noInterrupts();
	Serial.begin(115200);
	delay(50);
	Serial.println("------------------");
	Serial.println("INIT");
	InitEEPROM(StorageAdress_EEPROMMax);
	SetupResetProcedures();
	SetupLeds();
	SetupWiFi();
	SetupSSDP();
	Serial.println("INIT complete");
	interrupts();
}

void SetupResetProcedures()
{
	pinMode(interruptPinReset, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPinReset), HandleResetInterrupt, CHANGE);
}

void SetupWiFi()
{
	Serial.println("SetupWiFi");
	Server.on("/", handleRoot);
	Server.on("", handleRoot);
	//AUTOCONNECT_USE_PREFERENCES; //backward compatibility with ESP2866
	AutoConnectConfig acConfig;
	acConfig.autoReconnect = true;
	acConfig.ticker = true;
	acConfig.boundaryOffset = StorageAdress_AutoConnect;
	acConfig.title = HomeLEDTitle + "Menu";
	acConfig.apid = GenerateDefaultHostname();
	acConfig.hostName = ReadValidHostname();
	acConfig.psk = DefaultPassword;
	Portal.config(acConfig);
	IsServerReady = Portal.begin();
	if (IsServerReady)
	{
		Serial.println("\tWiFi connected");
		Serial.print("\tSSID: ");
		Serial.println(WiFi.SSID());
		Serial.print("\tIP address: ");
		Serial.println(WiFi.localIP());
		Serial.print("\tHostname: ");
		Serial.println(wifi_station_get_hostname());
		Serial.println("\tWebServer Started");
	}
	else
	{
		Serial.println("WebServer NOT ready, unknown error.");
	}
}

void SetupSSDP()
{
	Serial.println("SetupSSDP");
	//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266SSDP/examples/SSDP/SSDP.ino
	Server.on("/index.html", HTTP_GET, []() { Server.send(200, "text/plain", "Hello World!"); });
	Server.on("/description.xml", HTTP_GET, []() { Serial.println("\tSSDP Request"); SSDP.schema(Server.client()); });
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
	SSDP.setName(ReadValidHostname());
	Serial.print("\t");
	Serial.print("Model: ");
	Serial.println(ModelName);
	Serial.print("Serial: ");
	Serial.println(serialNo);
	Serial.print("Name: ");
	Serial.println(ReadValidHostname());
	if (SSDP.begin())
	{
		Serial.println("\tSSDP started");
	}
	else
	{
		Serial.println("\tERROR starting SSDP");
	}
}

void SetupLeds()
{
	Serial.println("SetupLeds");
	leds.begin();
	leds.clear();
	leds.show();
	RestoreConfig();
	if (CurrentMode == NULL)
	{
		if (!SetMode(StartMode))
		{
			Serial.println("\tError SetMode");
		}
	}
	Serial.print("\t");
	LEDsStart();
}

#pragma endregion

void handleRoot()
{
	String Return;
	for (size_t i = 0; i < Server.args(); i++)
	{
		auto argName = Server.argName(i);
		auto argVal = Server.arg(i);
		if (argName == "get")
		{
			Return += CurrentConfig2String();
		}
		else if (argName == "config")
		{
			if (argVal == "save")
			{
				Return += StoreConfig();
			}
			else if (argVal == "load")
			{
				Return += RestoreConfig();
			}
			else if (argVal == "clear")
			{
				Return += ClearConfigMemory();
			}
		}
		else if (argName == "setHostname")
		{
			StoreHostname(argVal);
		}
		else
		{
			Return += SetProperty(argName, argVal);
		}
	}
	if (Server.args() == 0)
	{
		digitalWrite(BuiltInLed, LOW); //Led port einschlaten
		Return = "Welcome to the server";
		delay(100);
		digitalWrite(BuiltInLed, HIGH); //Led port ausschalten
	}
	Server.send(200, "text/plain", Return);
	Serial.println(Return);
}

void loop(void)
{
	if (IsServerReady)
	{
		Portal.host().handleClient();
		Portal.handleClient();
		//TODO not sure if both is neccesary
	}
	delay(100);
}

//Function is called by the timer multiple times a second
void RefreshLeds(void* pArg)
{
	CurrentMode->NextState();
	leds.show();
}
//TODO try to extract the following to other files
#pragma region Led Functs

void LEDsStart()
{
	if (LEDsStarted)
	{
		return;
	}
	UpdateNumOfLeds(CurrentNumberOfLeds);
	UpdateBri(CurrentBrigthnes);
	UpdateSpeed(CurrentLEDRefreshTime);
	os_timer_setfn(&ShowTimer, RefreshLeds, NULL);
	os_timer_arm(&ShowTimer, CurrentLEDRefreshTime, true);
	Serial.println("LEDs started");
	LEDsStarted = true;
}

void LEDsStop()
{
	if (!LEDsStarted)
	{
		return;
	}
	os_timer_disarm(&ShowTimer);
	delayMicroseconds(CurrentLEDRefreshTime + 20);
	Serial.println("LEDs stopped");
	leds.clear();
	leds.show();
	LEDsStarted = false;
}

bool SetMode(String s)
{
	auto TempMode = CurrentMode;
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
	else if (CurrentMode != NULL && s == CurrentMode->ID())
	{
		return false;
	}
	else if (s == "rainbow")
	{
		CurrentMode = new RainbowMode(&leds);
	}
	else if (s == "color")
	{
		CurrentMode = new ColorMode(&leds);
	}
	else if (s == "sin")
	{
		CurrentMode = new SinMode(&leds);
		CurrentMode->Set("svo", "20");
		CurrentMode->Set("sv", "3");
	}
	else if (s == "pixel")
	{
		CurrentMode = new OnePixelMode(&leds);
	}
	else if (s == "doors")
	{
		int maxpin = D2 > D3 ? D2 : D3;
		maxpin = maxpin > D4 ? maxpin : D4;
		maxpin = maxpin > D5 ? maxpin : D5;
		int* PinDoorMap = new int[maxpin];
		for (size_t i = 0; i < maxpin; i++)
		{
			PinDoorMap[i] = -1;
		}
		PinDoorMap[D2] = 0;
		PinDoorMap[D3] = 1;
		PinDoorMap[D4] = 2;
		PinDoorMap[D5] = 3;
		CurrentMode = new DoorsMode(&leds, 4, PinDoorMap);
	}
	else
	{
		return false;
	}
	delete TempMode;
	return true;
}

bool UpdateSpeed(int newspeed)
{
	if (newspeed < 20)
	{
		newspeed = 20;
	}
	else if (newspeed > 10000)
	{
		newspeed = 10000;
	}
	if (newspeed != CurrentLEDRefreshTime)
	{
		CurrentLEDRefreshTime = newspeed;
		LEDsStop();
		LEDsStart();
		return true;
	}
	return false;
}

bool UpdateNumOfLeds(int newnum)
{
	if (newnum < 1)
	{
		newnum = 1;
	}
	else if (newnum > 150)
	{
		newnum = 150;
	}
	if (CurrentNumberOfLeds != newnum)
	{
		CurrentNumberOfLeds = newnum;
		leds.clear();
		leds.updateLength(CurrentNumberOfLeds);
		for (size_t i = newnum; i < 150; i++)
		{
			leds.setPixelColor(0, 0);
		}
		return true;
	}
	return false;
}

bool UpdateBri(int newbri)
{
	if (newbri < 1)
	{
		newbri = 1;
	}
	else if (newbri > 255)
	{
		newbri = 255;
	}
	if (CurrentBrigthnes != newbri)
	{
		CurrentBrigthnes = newbri;
		leds.setBrightness(newbri);
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
	Serial.println("SetProperty (" + argName + ")=(" + argVal + ")");
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
		String s = argVal;
		if (SetMode(s))
		{
			Return += "Changed Mode to: " + s + "\n";
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
	Return += "m=" + String(CurrentMode->ID()) + "&";
	for (size_t i = 0; i < CurrentMode->NumberofParams(); i++)
	{
		auto parname = CurrentMode->GetName(i);
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
	} while (param != "");
	return result;
}

String GetValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++) {
		if (data.charAt(i) == separator || i == maxIndex) {
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
		return "SUCCESS storing Config";
	}
	else
	{
		return "ERROR storing Config";
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
		Serial.println("Stored Hostname was empty");
		hostname = GenerateDefaultHostname();
	}
	return hostname;
}

String StoreHostname(String hostname)
{
	Serial.print("storing new hostname: ");
	Serial.println(hostname);
	if (WriteEEPROM(StorageAdress_Start_Hostname, hostname, StorageAdress_End_Hostname))
	{
		//SetupSSDP();
		//SetupWiFi();
		SSDP.end();
		SSDP.setName(ReadValidHostname());
		SSDP.begin();
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
void HandleResetInterrupt()
{
	if (digitalRead(interruptPinReset) == HIGH) //released
	{
		auto pressedTime = millis() - ResetInitiatedAt;
		if (pressedTime > ResetPressedTime)
		{
			Serial.println("Reset Button released after time. Reset and cleaning memory now!");
			//ClearConfigMemory();
			ClearEEPROM(0, StorageAdress_EEPROMMax); //geht auch nicht
			//auto credential = AutoConnectCredential(0);
			//AutoConnect::;
			AutoConnectCredential credential;
			station_config_t config;
			uint8_t ent = credential.entries();
			Serial.print("Test10: ");
			Serial.println(ent); //TODO geht noch nicht. gibt 0 zurück
			//https://hieromon.github.io/AutoConnect/credit.html#constructors
			while (ent > 0)
			{
				credential.load((int8_t)0, &config);
				Serial.print("Clearing WiFi Credentials for:");
				Serial.println((const char*)&config.ssid[0]);
				credential.del((const char*)&config.ssid[0]);
				ent--;
			}
			ESP.restart(); // macht millis() unbrauchbar
		}
		else
		{
			Serial.println("Abort Reset. (" + String(pressedTime) + ") milliseconds pressed");
		}
	}
	else //pressed
	{
		Serial.println("Reset Button pressed. Releasing it in more then 4 sec will reset the module.");
		ResetInitiatedAt = millis();
	}
}

#pragma endregion