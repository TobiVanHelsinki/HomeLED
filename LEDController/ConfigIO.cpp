#include "ConfigIO.h"

String ConfigIO::StoreConfig(String config)
{
	if (WriteFile(FileConfig, config))
	//if (WriteEEPROM(StorageAdress_Start_Configuration, config, StorageAdress_End_Configuration))
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
	if (TruncateFile(FileConfig))
	//if (ClearEEPROM(StorageAdress_Start_Configuration, StorageAdress_End_Configuration))
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
	auto hostname = ReadFile(FileHostName);
	//auto hostname = ReadEEPROM(StorageAdress_Start_Hostname);
	if (hostname.isEmpty())
	{
		SERIALWRITELINE("Stored Hostname was empty, return new generative hostname");
		hostname = GenerateDefaultHostname();
	}
	else
	{
		SERIALWRITE("Your Hostname is: ");
		SERIALWRITELINE(hostname);
	}
	return hostname;
}

String ConfigIO::StoreHostname(String hostname)
{
	SERIALWRITE("storing new hostname:-");
	SERIALWRITE(hostname);
	SERIALWRITELINE("-");
	if (WriteFile(FileHostName, hostname))
	//if (WriteEEPROM(StorageAdress_Start_Hostname, hostname, StorageAdress_End_Hostname))
	{
		return "SUCCESS storing Hostname";
	}
	else
	{
		return "ERROR storing Hostname";
	}
}
