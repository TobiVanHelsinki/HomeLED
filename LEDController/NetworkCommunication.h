#pragma once
#include <AutoConnect.h>
#include <PageStream.h>
#include <PageBuilder.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266SSDP.h>
#include <AutoConnectCredential.h>
#include "ProjectHeader.h"
#include "LedFunctions.h"
#include "ConfigIO.h"

class NetworkCommunication
{
	static ESP8266WebServer Server;
	static AutoConnectUpdate UpdateService;
public:
	static bool IsServerReady;
	static AutoConnect Portal;
	static void SetupWiFi();

	static void portalNotFound();

	static bool startCP(IPAddress ip);

	static void onUpdateStart();

	static void onUpdateEnd();

	static void onUpdateProgress(int p, int p1);

	static	void onUpdateError(int code);

	static void SetupSSDP();

	static void handleRoot();
};
