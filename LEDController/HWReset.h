#pragma once
#include "ProjectHeader.h"
#include "ConfigIO.h"
#include <AutoConnect.h>

class HWReset
{
public:
	static unsigned long ResetInitiatedAt;
	static void ProcessResetInterrupt();
	static void ResetSystem();
};
