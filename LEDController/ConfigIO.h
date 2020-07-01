#pragma once
#include "ProjectHeader.h"
#include "EEPROMHelper.h"

class ConfigIO
{
public:
	static String StoreConfig(String config);

	static String ClearConfigMemory();

	static String GenerateDefaultHostname();

	static String ReadValidHostname();

	static String StoreHostname(String hostname);
};
