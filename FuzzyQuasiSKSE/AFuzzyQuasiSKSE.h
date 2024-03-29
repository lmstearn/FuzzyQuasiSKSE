#ifndef __cplusplus
#error A C++ compiler is required!
#endif
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows specific: C, C++
#include <windows.h>
#include <winsock2.h>
#endif


#include "targetver.h" //SDK versioning


int i, retVal;
// AFuzzyQuasiSKSE.cpp: Defines the entry point for the application.
//
#include "AInfoLoad.h"
static wchar_t* myVar;
static wchar_t* myVar1;
class LVTextMem
{
	const int allocSize = MAX_LOADSTRING;
public:
	wchar_t* Var()
	{
		if (myVar)
			return myVar;
		else
		{
			myVar = (wchar_t*)calloc(allocSize, SIZEOF_WCHAR);
			return myVar;
		}
	}
	~LVTextMem()
	{
		if (myVar && *myVar != NULL)
		{
			free(myVar);
			myVar = nullptr;
		}
	}
	void ReleaseVar()
	{
		if (myVar && *myVar != NULL)
		{
			free(myVar);
			myVar = nullptr;
		}
	}
};
class SSEPathMem
{
	const int allocSize = RCDATALIM;
public:
	wchar_t* Var()
	{
		if (myVar1)
			return myVar1;
		else
		{
			myVar1 = (wchar_t*)calloc(allocSize, SIZEOF_WCHAR);
			return myVar1;
		}
	}
	~SSEPathMem()
	{
		if (myVar1  && *myVar1 != NULL)
		{
			free(myVar1);
			myVar1 = nullptr;
		}
	}
	void ReleaseVar()
	{
		if (myVar1 && *myVar1 != NULL)
		{
			free(myVar1);
			myVar1 = nullptr;
		}
	}
};
class APP_CLASS
{
public:
	APP_CLASS();

	// This is the static callback that we register
	static INT_PTR CALLBACK s_DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// The static callback recovers the "this" pointer and then calls this member function.
	INT_PTR DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};



enum GroupState
{
	COLLAPSIBLE = 8,
	COLLAPSED = 1,
	EXPANDED = 0
};

struct TabData
{
	HWND page1, page2, page3;
	HWND tab;
} tabData;

