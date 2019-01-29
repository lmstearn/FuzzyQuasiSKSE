#pragma once

#include "IPrefix.h"
//#include "skse64/PapyrusNativeFunctions.h"

namespace AchievementsNS
{
	float DoMathFun(StaticFunctionTag* base, float fIn1, float fIn2);
	bool RegisterFuncs(VMClassRegistry* registry);
}
