#pragma once
#include "ProjectHeader.h"
#include <AutoConnect.h>

//void ICACHE_RAM_ATTR HWReset::ProcessResetInterrupt();

class HWReset
{
public:
	static void SetupResetProcedures();
	static unsigned long ResetInitiatedAt;
	static void ProcessResetInterrupt();
	static void ResetSystem();
};
