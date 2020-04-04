#include <ESP8266WebServer.h>
#include "PrivateWiFiInfo.h"
#include "RainbowMode.h"
#include "OnePixelMode.h"
#include "SinMode.h"
#include "ColorMode.h"
#include "DoorsMode.h"
#include <EEPROM.h>
#include <ESP8266WebServerSecure.h>
#include "LEDController.h"

const auto LEDsPin = D1;
const auto BuiltInLed = D4;

auto CurrentNumberOfLeds = 150;
auto CurrentLEDRefreshTime = 60;
auto CurrentBrigthnes = 100;
auto StartMode = "sin";

#pragma region Server Vars

#define WIFI_TIMEOUT 30000
ESP8266WebServer server(80);
bool ServerReady = false;

#pragma endregion
#pragma region Led Vars
Adafruit_NeoPixel leds = Adafruit_NeoPixel(CurrentNumberOfLeds, LEDsPin, NEO_GRB + NEO_KHZ800);
ModeBase* CurrentMode;
os_timer_t ShowTimer;
bool LEDsStarted = false;
#pragma endregion

#pragma region Setups

void setup() {
	Serial.begin(115200);
	delay(150);
	Serial.println("INIT");
	SetUpInterrupts();
	SetupLeds();
	SetupWiFi();
	Serial.println("INIT complete");
}

void SetupWiFi() {
	Serial.print("SetupWiFi, Connecting to ");
	Serial.println(WIFI_SSID);
	WiFi.hostname(WIFI_HOSTNAME);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	WiFi.mode(WIFI_STA);
#ifdef STATIC_IP  
	WiFi.config(ip, gateway, subnet);
#endif
	Serial.print("\tWaiting ");
	unsigned long connect_start = millis();
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");

		if (millis() - connect_start > WIFI_TIMEOUT) {
			Serial.println();
			Serial.print("\tTried ");
			Serial.print(WIFI_TIMEOUT);
			Serial.print(" ms. Resetting ESP now.");
			ESP.restart();
		}
	}
	if (WiFi.status() == WL_CONNECTED)
	{
		ServerReady = true;
		Serial.println();
		Serial.println("\tWiFi connected");
		Serial.print("\tIP address: ");
		Serial.println(WiFi.localIP());
		Serial.print("\tHostname: ");
		Serial.println(wifi_station_get_hostname());
	}
	else
	{
		ServerReady = false;
	}
	server.on("/", handleRoot);
	server.on("", handleRoot);
	server.begin();
}

void SetupLeds()
{
	Serial.println("SetupLeds");
	leds.begin();
	leds.clear();
	leds.show();
	LoadConfig();
	if (CurrentMode == NULL)
	{
		if(!SetMode(StartMode))
		{
			Serial.println("\tError SetMode");
		}
	}
	Serial.print("\t");
	LEDsStart();
}

void SetUpInterrupts()
{
	Serial.println("SetupInterrupts");
	pinMode(D2, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(D2), D2Change, CHANGE);
	pinMode(D3, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(D3), D3Change, CHANGE);
	pinMode(D4, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(D4), D4Change, CHANGE);
	pinMode(D5, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(D5), D5Change, CHANGE);
}

void DeSetUpInterrupts()
{ 
	Serial.println("DeSetupInterrupts");
	detachInterrupt(digitalPinToInterrupt(D2));
	detachInterrupt(digitalPinToInterrupt(D3));
}

#pragma endregion

#pragma region Server Funct
void handleRoot()
{
	String Return;
	for (size_t i = 0; i < server.args(); i++)
	{
		auto argName = server.argName(i);
		auto argVal = server.arg(i);
		if (argName == "get")
		{
			Return += CurrentConfigToString();
		}
		else if (argName == "config")
		{
			if (argVal == "save")
			{
				Return += SaveConfig();
			}
			else if (argVal == "load")
			{
				Return += LoadConfig();
			}
			else if (argVal == "clear")
			{
				Return += ClearEEPROM();
			}
		}
		else
		{
			Return += SetProperty(argName, argVal);
		}
	}
	if (server.args() == 0)
	{
		digitalWrite(BuiltInLed, LOW); //Led port einschlaten
		Return = "Welcome to the server";
		delay(100);
		digitalWrite(BuiltInLed, HIGH); //Led port ausschalten
	}
	server.send(200, "text/plain", Return);
	Serial.print(Return);
}
#pragma endregion
#pragma region Configuration
String SetProperty(String argName, String argVal)
{
	String Return;
	Serial.println("SetProperty (" + argName + ")=(" + argVal + ")");
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
		Return += CurrentMode->Set(argName, argVal);
	}
	return Return;
}

String CurrentConfigToString()
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
#pragma endregion
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
#pragma region Interrupt Functs
void ICACHE_RAM_ATTR D2Change() { if (CurrentMode) { CurrentMode->Interrupt(D2); } }
void ICACHE_RAM_ATTR D3Change() { if (CurrentMode) { CurrentMode->Interrupt(D3); } }
void ICACHE_RAM_ATTR D4Change() { if (CurrentMode) { CurrentMode->Interrupt(D4); } }
void ICACHE_RAM_ATTR D5Change() { if (CurrentMode) { CurrentMode->Interrupt(D5); } }
#pragma endregion

void loop(void)
{
	if (ServerReady)
	{
		server.handleClient();
	}
	delay(3000);
}

void RefreshLeds(void *pArg)
{
	CurrentMode->NextState();
	leds.show();
}
#pragma region EEPROM

String LoadConfig()
{
	Serial.println("LoadConfig");
	String config;
	EEPROM.begin(512);
	auto count = EEPROM.read(0);
	for (size_t i = 0; i < count; i++)
	{
		auto read = (char)EEPROM.read(i + 1);
		config += read;
	}
	EEPROM.end();
	Serial.println("Recovered following settings:" + config);
	String param;
	int counter = 0;
	if (config == NULL || config == "")
	{
		return "no config in memory";
	}
	String Ret;
	do
	{
		param = getValue(config, '&', counter);
		counter++;
		auto name = getValue(param, '=', 0);
		auto value = getValue(param, '=', 1);
		Ret += SetProperty(name, value);
	}
	while (param != "");
	return Ret;
}
String getValue(String data, char separator, int index)
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
String SaveConfig()
{
	String config = CurrentConfigToString();
	Serial.println("SaveConfig: " + config);

	unsigned char* configbytes = new unsigned char[config.length()];
	config.getBytes(configbytes, config.length());

	EEPROM.begin(512);
	EEPROM.write(0, config.length());
	for (size_t i = 0; i < config.length(); i++)
	{
		EEPROM.write(1 + i, configbytes[i]);
	}
	EEPROM.commit();
	EEPROM.end();
	Serial.println("SaveConfig Done");
	return "SaveConfig Done\n";
}

String ClearEEPROM() 
{
	EEPROM.begin(512);
	// write a 0 to all 512 bytes of the EEPROM
	for (int i = 0; i < 512; i++)
	{
		EEPROM.write(i, 0);
	}
	EEPROM.end();
	return "cleared memory\n";

}
#pragma endregion
