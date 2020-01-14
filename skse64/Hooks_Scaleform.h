#pragma once

#include <new>
#include <list>

#include "skse64_common/SafeWrite.h"
#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/skse_version.h"
#include "common/IMemPool.h"
#include "xbyak/xbyak.h"

#include "skse64/GlobalLocks.h"
#include "skse64/HashUtil.h"
#include "skse64/ScaleformExtendedData.h"
#include "skse64/ScaleformLoader.h"
#include "skse64/GameMenus.h"
#include "skse64/GameAPI.h"
#include "skse64/Hooks_Gameplay.h"
#include "skse64/PapyrusEvents.h"
#include "skse64/PapyrusVM.h"
#include "skse64/PluginManager.h"

void SetLastControlDown(const char * control, UInt32 keycode);
void SetLastControlUp(const char * control, UInt32 keycode);

void Hooks_Scaleform_Commit(void);

bool RegisterScaleformPlugin(const char * name, SKSEScaleformInterface::RegisterCallback callback);
void RegisterScaleformInventory(SKSEScaleformInterface::RegisterInventoryCallback callback);
