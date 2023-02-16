#pragma once
#define _WINSOCKAPI_


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows specific: C, C++
#include <windows.h>
#include <winsock2.h>
#endif

#include "targetver.h" //SDK versioning

// C RunTime Header Files
#include <stdlib.h>
// #include <stdio.h> // in strsafe.h
//#include <string.h> // in strsafe.h
#include <strsafe.h>
#include <shellapi.h>
//#include <malloc.h> // in stdlib.h

//Listview
#include <CommCtrl.h>

// C++ Header Files:
#include <memory>
//#include <vector> // included in filesystem
//#include <string> //string included in iTypes
#include <filesystem>
//#include <iostream> //iostream included in iTypes
// #include <algorithm> // included in filesystem
// TODO: reference additional headers your program requires here

#include "resource.h"
#include "AIPrefix.h"

/* This guard cause random compilation errors
#ifndef AINFOLOAD_H
#include "AInfoLoad.h"
#define AINFOLOAD_H
#endif
*/
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

//Suppress warning on unused variables https://stackoverflow.com/a/1486931/2128797
#define UNUSED(expr) do { (void)(expr); } while (0)

// Consider constexpr unsigned int instead of #define value
//
//#define CSIDL_MYDOCUMENTS		0x0005 //My Documents (c4005 macro redef)
#define DWL_MSGRESULT				0
#define COL_SELLIM						200
#define MAX_TABS						10
#define MAX_LOADSTRING			255
#define RCDATALIM						2500 //compatible with SIZEOF(bytes)
#define MAX_GUIMOVPERSESS		100
#define MAX_FILE							4194304


//Aspect ratios over 9
#define FOURTHREEVID					12
#define WIDESCREENVID				16
#define UNIVISIUM						18 // e.g. Galaxy Tabs
#define ULTRAWIDEVID					21
#define DESIGNSCRX						1360
#define DESIGNSCRY						768

//Error Codes
// Global Variables:
UINT const WM_COPYGLOBALDATA = 0x0049; //Drop files filter
const wchar_t TEMP_CLASS_NAME[14] = L"ResCheckClass";
extern BOOL isLoading;
extern BOOL oneCallAutoSizeCols[MAX_TABS];

//typedef std::vector<std::int16_t> intVec;
typedef std::vector<std::wstring> LVA;
typedef std::vector<LVA> LVA2D;
const uint16_t maxInt = 32767;
const LONGLONG ID_LIST = 200;
wchar_t const EOL_DELIM = L'\n';
wchar_t const COMMA_DELIM = L',';
int const SIZEOF_WCHAR = sizeof(wchar_t);
//extern int i, retVal; //avoids LNK2005
extern wchar_t const* lPref;
extern int i; //avoids LNK2005
extern int retVal; //avoids LNK2005
extern float resX, resY;

//#define ITOW(A) Itow((A), &(char[16]){ 0 })
void FormatItowNotify(int a, wchar_t* buffer = nullptr);
wchar_t* ReallocateMem(wchar_t* aSource, int Size);
void ErrorRep(LPCWSTR lpszFunction, wchar_t* extraInf = nullptr, int var = maxInt);
void ErrorExit(LPCWSTR lpszFunction, LPCWSTR var, bool reportVar = FALSE);
int ProcessCompressedFile(wchar_t* tempDest, const char command7z);