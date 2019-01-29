#pragma once


// 4018 - signed/unsigned mismatch
// 4200 - zero-sized array
// 4244 - loss of data by assignment
// 4267 - possible loss of data (truncation)
// 4305 - truncation by assignment
// 4288 - disable warning for crap microsoft extension screwing up the scope of variables defined in for loops
// 4311 - pointer truncation
// 4312 - pointer extension
// 4596	(level 4) identifier': illegal qualified name in member declaration
#pragma warning(disable: 4018 4200 4244 4267 4305 4288 4312 4311 4596)

// winxp and above
//#define _WIN32_WINNT	0x0501


#include <winsock2.h>
#include <stdio.h>
#include "7Zip\Z7zFile.h"
#include "7Zip\ZLzmaDec.h"
#include "7Zip\ZAlloc.h"
#include "7Zip\ZCpuArch.h"
#include "7Zip\7z.h"
#include "7Zip\Z7zBuf.h"
#include "7Zip\Z7zCrc.h"
#include "7Zip\7zVersion.h"

//default compression at 16k
#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 24) //16777216 or default 16mb
//#define UNUSED_VAR(VAR) VAR
static void *SzAlloc(ISzAllocPtr p, size_t size) { UNUSED_VAR(p); return MyAlloc(size); }
static void SzFree(ISzAllocPtr p, void *address) { UNUSED_VAR(p); MyFree(address); }
int Do7zFile(int numargs, const char *arg[3]);
static SRes PrintString(const wchar_t *s);


/*
	In: 
  dest     - output data
	 destLen  - output data size or Dictionary size
	  src      - input data
	  srcLen   - input data size
	  propData - LZMA properties  (5 bytes)
	  propSize - size of propData buffer (5 bytes)
	  finishMode - It has meaning only if the decoding reaches output limit (*destLen).
	  LZMA_FINISH_ANY - Decode just destLen bytes.
	  LZMA_FINISH_END - Stream must be finished after (*destLen).
	  You can use LZMA_FINISH_END, when you know that 
	  current output buffer covers last bytes of stream. 
	  alloc    - Memory allocator.

	  Out: 
  destLen  - processed output size 
	  srcLen   - processed input size 

	  Output:
  SZ_OK
	  status:
  LZMA_STATUS_FINISHED_WITH_MARK
	  LZMA_STATUS_NOT_FINISHED 
	  LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK
	  SZ_ERROR_DATA - Data error
	  SZ_ERROR_MEM  - Memory allocation error
	  SZ_ERROR_UNSUPPORTED - Unsupported properties
	  SZ_ERROR_INPUT_EOF - It needs more bytes in input buffer (src).
	  */

#define MY_STDAPI int MY_STD_CALL
MY_STDAPI LzmaUncompress(unsigned char *dest, size_t *destLen, const unsigned char *src, size_t *srcLen, 	const unsigned char *props, size_t propsSize, ELzmaStatus status);



//SRes LzmaDecode(Byte *dest, SizeT *destLen, const Byte *src, SizeT *srcLen,	const Byte *propData, unsigned propSize, ELzmaFinishMode finishMode, 	ELzmaStatus *status, ISzAllocPtr alloc);
//static SRes Decode(ISeqOutStream *outStream, ISeqInStream *inStream);
/*
// common
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common\ITypes.h"
#include "common\IErrors.h"
#include "common\IDynamicCreate.h"
#include "common\IDebugLog.h"
#include "common\ISingleton.h"
// skse64_common

#include "skse64_common\BranchTrampoline.h"
#include "skse64_common\Relocation.h"
#include "skse64_common\SafeWrite.h"
#include "skse64_common\skse_version.h"
#include "skse64_common\Utilities.h"

// skse64
#include "skse64\BSModelDB.h"
#include "skse64\Colors.h"
#include "skse64\CustomMenu.h"
#include "skse64\GameAPI.h"
#include "skse64\GameBSExtraData.h"
#include "skse64\GameCamera.h"
#include "skse64\GameData.h"
#include "skse64\GameEvents.h"
#include "skse64\GameExtraData.h"
#include "skse64\GameFormComponents.h"
#include "skse64\GameForms.h"
#include "skse64\GameHandlers.h"
#include "skse64\GameInput.h"
#include "skse64\GameMenus.h"
#include "skse64\GameObjects.h"
#include "skse64\GamePathing.h"
#include "skse64\GameReferences.h"
#include "skse64\GameResources.h"
#include "skse64\GameRTTI.h"
#include "skse64\GameSettings.h"
#include "skse64\GameStreams.h"
#include "skse64\gamethreads.h"
#include "skse64\GameTypes.h"
#include "skse64\GameUtilities.h"
#include "skse64\GlobalLocks.h"
#include "skse64\HashUtil.h"
#include "skse64\Hooks_Camera.h"
#include "skse64\Hooks_Data.h"
#include "skse64\Hooks_Debug.h"
#include "skse64\Hooks_Diagnostics.h"
#include "skse64\Hooks_DirectInput8Create.h"
#include "skse64\Hooks_Event.h"
#include "skse64\Hooks_Gameplay.h"
#include "skse64\Hooks_Handlers.h"
#include "skse64\Hooks_Memory.h"
#include "skse64\Hooks_NetImmerse.h"
#include "skse64\Hooks_ObScript.h"
#include "skse64\Hooks_Papyrus.h"
#include "skse64\Hooks_SaveLoad.h"
#include "skse64\Hooks_Scaleform.h"
#include "skse64\Hooks_Threads.h"
#include "skse64\Hooks_UI.h"
#include "skse64\InputMap.h"
#include "skse64\InternalSerialization.h"
#include "skse64\InternalTasks.h"
#include "skse64\NiAdditionalGeometryData.h"
#include "skse64\NiAllocator.h"
#include "skse64\NiControllers.h"
#include "skse64\NiExtraData.h"
#include "skse64\NiGeometry.h"
#include "skse64\NiInterpolators.h"
#include "skse64\NiLight.h"
#include "skse64\NiMaterial.h"
#include "skse64\NiNodes.h"
#include "skse64\NiObjects.h"
#include "skse64\NiProperties.h"
#include "skse64\NiRenderer.h"
#include "skse64\NiRTTI.h"
#include "skse64\NiSerialization.h"
#include "skse64\NiTextures.h"
#include "skse64\NiTypes.h"
#include "skse64\ObScript.h"
#include "skse64\PapyrusActiveMagicEffect.h"
#include "skse64\PapyrusActor.h"
#include "skse64\PapyrusActorBase.h"
#include "skse64\PapyrusActorValueInfo.h"
#include "skse64\PapyrusAlias.h"
#include "skse64\PapyrusAmmo.h"
#include "skse64\PapyrusArgs.h"
#include "skse64\PapyrusArmor.h"
#include "skse64\PapyrusArmorAddon.h"
#include "skse64\PapyrusArt.h"
#include "skse64\PapyrusBook.h"
#include "skse64\PapyrusCamera.h"
#include "skse64\PapyrusCell.h"
#include "skse64\PapyrusClass.h"
#include "skse64\PapyrusColorForm.h"
#include "skse64\PapyrusCombatStyle.h"
#include "skse64\PapyrusConstructibleObject.h"
#include "skse64\PapyrusDefaultObjectManager.h"
#include "skse64\PapyrusDelayFunctors.h"
#include "skse64\PapyrusEnchantment.h"
#include "skse64\PapyrusEquipSlot.h"
#include "skse64\PapyrusEventFunctor.h"
#include "skse64\PapyrusEvents.h"
#include "skse64\PapyrusFaction.h"
#include "skse64\PapyrusFlora.h"
#include "skse64\PapyrusForm.h"
#include "skse64\PapyrusFormList.h"
#include "skse64\PapyrusGame.h"
#include "skse64\PapyrusGameData.h"
#include "skse64\PapyrusHeadPart.h"
#include "skse64\PapyrusIngredient.h"
#include "skse64\PapyrusInput.h"
#include "skse64\PapyrusInterfaces.h"
#include "skse64\PapyrusKeyword.h"
#include "skse64\PapyrusLeveledActor.h"
#include "skse64\PapyrusLeveledItem.h"
#include "skse64\PapyrusLeveledSpell.h"
#include "skse64\PapyrusMagicEffect.h"
#include "skse64\PapyrusMath.h"
#include "skse64\PapyrusMisc.h"
#include "skse64\PapyrusModEvent.h"
#include "skse64\PapyrusNativeFunctions.h"
#include "skse64\PapyrusNetImmerse.h"
#include "skse64\PapyrusObjectReference.h"
#include "skse64\PapyrusObjects.h"
#include "skse64\PapyrusPerk.h"
#include "skse64\PapyrusPotion.h"
#include "skse64\PapyrusQuest.h"
#include "skse64\PapyrusRace.h"
#include "skse64\PapyrusReferenceAlias.h"
#include "skse64\PapyrusScroll.h"
#include "skse64\PapyrusShout.h"
#include "skse64\PapyrusSKSE.h"
#include "skse64\PapyrusSound.h"
#include "skse64\PapyrusSoundDescriptor.h"
#include "skse64\PapyrusSpawnerTask.h"
#include "skse64\PapyrusSpell.h"
#include "skse64\PapyrusStringUtil.h"
#include "skse64\PapyrusTextureSet.h"
#include "skse64\PapyrusTree.h"
#include "skse64\PapyrusUI.h"
#include "skse64\PapyrusUICallback.h"
#include "skse64\PapyrusUtility.h"
#include "skse64\PapyrusValue.h"
#include "skse64\PapyrusVM.h"
#include "skse64\PapyrusWeapon.h"
#include "skse64\PapyrusWeather.h"
#include "skse64\PapyrusWornObject.h"
#include "skse64\PluginAPI.h"
#include "skse64\PluginManager.h"
#include "skse64\ScaleformAPI.h"
#include "skse64\ScaleformCallbacks.h"
#include "skse64\ScaleformExtendedData.h"
#include "skse64\ScaleformLoader.h"
#include "skse64\ScaleformMovie.h"
#include "skse64\ScaleformState.h"
#include "skse64\ScaleformTypes.h"
#include "skse64\ScaleformValue.h"
#include "skse64\ScaleformVM.h"
#include "skse64\Serialization.h"
#include "skse64\Translation.h"

// skse64_loader
#include "skse64_loader\Options.h"
// skse64_loader_common
#include "skse64_loader_common\IdentifyEXE.h"
#include "skse64_loader_common\Inject.h"
#include "skse64_loader_common\LoaderError.h"
#include "skse64_loader_common\Steam.h"
// 
*/