#include "Achievements.h"

//NO UNICODE ALLOWED!
namespace AchievementsNS
{
	float DoMathFun(StaticFunctionTag* base, float fArg1, float fArg2)
	{
		// your own custom function to be used in papyrus scripts
		return fArg1 + fArg2;
	}

	bool RegisterFuncs(VMClassRegistry* registry)
	{
		//registry->RegisterFunction(new NativeFunction2 <StaticFunctionTag, float, float, float>("AchievementsNS_DoMathFun", "Utility", AchievementsNS::DoMathFun, registry));

		// example optional flag you can set
		//registry->SetFunctionFlags("Utility", "MyPlugin_DoMathFun", VMClassRegistry::kFunctionFlag_NoWait);

		return true;
	}
}