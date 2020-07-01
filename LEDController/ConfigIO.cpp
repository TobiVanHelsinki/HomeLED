#include "ConfigIO.h"

String ConfigIO::StoreConfig(String config)
{
	if (WriteEEPROM(StorageAdress_Start_Configuration, config, StorageAdress_End_Configuration))
	{
		return "SUCCESS storing Config";
	}
	else
	{
		return "ERROR storing Config";
	}
}

String ConfigIO::ClearConfigMemory()
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

String ConfigIO::GenerateDefaultHostname()
{
	return HomeLEDTitle + String(ESP.getChipId(), HEX);
}

String ConfigIO::ReadValidHostname()
{
	auto hostname = ReadEEPROM(StorageAdress_Start_Hostname);
	if (hostname.isEmpty())
	{
		SERIALWRITELINE("Stored Hostname was empty");
		hostname = GenerateDefaultHostname();
	}
	return hostname;
}

String ConfigIO::StoreHostname(String hostname)
{
	SERIALWRITE("storing new hostname:-");
	SERIALWRITE(hostname);
	SERIALWRITELINE("-");
	if (WriteEEPROM(StorageAdress_Start_Hostname, hostname, StorageAdress_End_Hostname))
	{
		//SetupSSDP();
		//SetupWiFi();
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