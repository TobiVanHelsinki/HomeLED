#include "NetworkCommunication.h"
bool NetworkCommunication::IsServerReady = false;
ESP8266WebServer NetworkCommunication::Server(HTTPPort);
AutoConnectUpdate NetworkCommunication::UpdateService("ts1", 8010);
AutoConnect NetworkCommunication::Portal(Server);

void NetworkCommunication::SetupWiFi()
{
	SERIALWRITELINE("SetupWiFi");
	Server.on("/", handleRoot);
	Server.on("", handleRoot);
	auto customname = NetworkCommunication::ReadValidHostname();
	AutoConnectConfig acConfig;
	//acConfig.boundaryOffset = StorageAdress_AutoConnect;
	acConfig.title = customname +" V" + Version;
	acConfig.apid = customname;
	acConfig.psk = DEFAULTPASSW;
	//acConfig.autoReconnect = false;
	//acConfig.autoReset = false;
	//acConfig.autoRise = true;
	//acConfig.autoSave = AC_SAVECREDENTIAL_AUTO;
	//AutoConnectUpdate update("apps.t-imperium.de/HomeLed", 80);
	acConfig.ota = AC_OTA_BUILTIN;
	acConfig.otaExtraCaption = Version;
	acConfig.portalTimeout = 0; //0=endless
	acConfig.hostName = customname;
	acConfig.apip = IPAddress(192, 168, 10, 1);
	acConfig.ticker = true;
	Portal.config(acConfig);
	//Portal.onDetect(startCP); // linking error after Version 1.1.7
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

void NetworkCommunication::portalNotFound()
{
	//SERIALWRITELINE("portal: Not found");
}

bool NetworkCommunication::startCP(IPAddress ip)
{
	LedFunctions::SetMode("cp");
	SERIALWRITELINE("CP (own WLAN) started, IP:" + WiFi.localIP().toString());
	return true;
}
void NetworkCommunication::onUpdateStart()
{
	SERIALWRITELINE("Updateprocess started");
}
void NetworkCommunication::onUpdateEnd()
{
	SERIALWRITELINE("Updateprocess ended");
}
void NetworkCommunication::onUpdateProgress(int p, int p1)
{
	SERIALWRITELINE("Update makes progress:" + String(p) + String(p1));
}
void NetworkCommunication::onUpdateError(int code)
{
	SERIALWRITELINE("Update encountered Error:" + String(code));
}

void NetworkCommunication::SetupSSDP()
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
	SSDP.setModelName(String(DeviceType)+ "-" +String(Version));
	SSDP.setModelURL(ModelURL);
	SSDP.setModelNumber(Version);
	auto serialNo = Version + String("_") + String(ESP.getChipId(), HEX);
	SSDP.setSerialNumber(serialNo);
	auto name = NetworkCommunication::ReadValidHostname();
	SSDP.setName(name);
	SERIALWRITE("\t");
	SERIALWRITE("Model: ");
	SERIALWRITELINE(DeviceType);
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

void NetworkCommunication::handleRoot()
{
	String result;
	bool shouldRestart = false;
	SERIALWRITELINE("-------handleRoot-------");
	if (Server.args() == 0)
	{
		SERIALWRITELINE("   Format Main Page-");
		char out[sizeof(Page_Main)+16+16+10];
		snprintf(out, sizeof(Page_Main), Page_Main, 
			NetworkCommunication::ReadValidHostname().c_str(), 
			WiFi.localIP().toString().c_str(), 
			Version);
		Server.send(200, "text/html", out);
	}
	else
	{
		for (auto i = 0; i < Server.args(); i++)
		{
			auto argName = Server.argName(i);
			if (argName.isEmpty())
			{
				continue;
			}
			auto argVal = Server.arg(i);
			SERIALWRITE("(");
			SERIALWRITE(argName);
			SERIALWRITE(")=(");
			SERIALWRITE(argVal);
			SERIALWRITELINE(")");
			if (argName == "get")
			{
				result += LedFunctions::CurrentConfig2String() + "&";
			}
			else if (argName == "config")
			{
				if (argVal == "save")
				{
					if (WriteFile(FileConfig1, LedFunctions::CurrentConfig2String()))
					{
						result += "SUCCESS storing Config&";
					}
					else
					{
						result += "ERROR storing Config&";
					}
				}
				else if (argVal == "load")
				{
					result += LedFunctions::String2CurrentConfig(ReadFile(FileConfig1));
				}
				else if (argVal == "clear")
				{
					if (TruncateFile(FileConfig1))
					{
						result += "SUCCESS cleaning Config&";
					}
					else
					{
						result += "ERROR cleaning Config&";
					}
				}
			}
			else if (argName == "hostname")
			{
				if (!argVal.isEmpty())
				{
					if (WriteFile(FileHostName, argVal))
					{
						result += "SUCCESS storing Hostname&";
					}
					else
					{
						result += "ERROR storing Hostname&";
					}
					SSDP.end();
					SSDP.setName(NetworkCommunication::ReadValidHostname());
					SSDP.begin();
					result += "restarted SSDP Server&";
				}
				result += "hostname=" + NetworkCommunication::ReadValidHostname() + "&";
			}
			else if (argName == "restart")
			{
				shouldRestart = true;
				result += "restarting module";
			}
			else
			{
				result += LedFunctions::HandleProperty(argName, argVal);
			}
		}
		if (WriteFile(FileLastConfig, LedFunctions::CurrentConfig2String()))
		{
			result += "SUCCESS storing current Config&";
		}
		else
		{
			result += "ERROR storing current CurrentConfig&";
		}
		Server.send(200, "text/plain", result);
	}

	SERIALWRITELINE(result);
	SERIALWRITELINE("-------Roothandled------");
	if (shouldRestart)
	{
		SERIALWRITELINE("Restarting now");
		ESP.restart();
	}
}

String NetworkCommunication::GenerateDefaultHostname()
{
	return DeviceType + String("-") + String(ESP.getChipId(), HEX);
}

String NetworkCommunication::ReadValidHostname()
{
	auto hostname = ReadFile(FileHostName);
	//auto hostname = ReadEEPROM(StorageAdress_Start_Hostname);
	if (hostname.isEmpty())
	{
		SERIALWRITE("Stored Hostname was empty, return new generative hostname:");
		hostname = GenerateDefaultHostname();
	}
	else
	{
		SERIALWRITE("Your Hostname is: ");
	}
	SERIALWRITELINE(hostname);
	return hostname;
}
