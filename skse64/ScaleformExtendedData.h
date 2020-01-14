#pragma once
#include "skse64/ScaleformCallbacks.h"
#include "skse64/ScaleformMovie.h"
#include "skse64/GameData.h"
#include "skse64/GameForms.h"
#include "skse64/GameObjects.h"
#include "skse64/GameExtraData.h"
#include "skse64/GameRTTI.h"
#include "skse64/GameMenus.h"


class TESForm;
class ActiveEffect;
class GFxMovieView;
class GFxValue;
class BGSKeywordForm;
class InventoryEntryData;
struct AlchemyEffectCategory;


void RegisterNumber(GFxValue * dst, const char * name, double value);
void RegisterBool(GFxValue * dst, const char * name, bool value);
void RegisterString(GFxValue * dst, GFxMovieView * view, const char * name, const char * str);
void RegisterKeywords(GFxValue * pFxVal, GFxMovieView * view, BGSKeywordForm * keywordForm);

// Item Card Extensions
namespace scaleformExtend
{
	void CommonItemData(GFxValue* pFxVal, TESForm * pForm);
	void StandardItemData(GFxValue* pFxVal, TESForm * pForm, InventoryEntryData * pEntry = NULL);
	void MagicItemData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive);
	void ActorData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive);
	void FormListData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive);
	void MiscData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive);
	void FormData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive);
	void InventoryData(GFxValue * pFxVal, GFxMovieView * movieView, InventoryEntryData * objDesc);

	void ItemInfoData(GFxValue* pFxVal, InventoryEntryData * pEntry);

	void CraftDisenchantData(GFxValue * pFxVal, GFxMovieView * movieView, InventoryEntryData * pEntry);
	void AlchemyCategoryArgs(AlchemyEffectCategory* effectArray, GFxValue * args, UInt32 numArgs);
};
