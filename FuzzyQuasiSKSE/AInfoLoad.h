#pragma once
#include "stdafx.h"
#include <strsafe.h>
#include <vector>
#include <string>
#include <filesystem>
#include <shellapi.h>
#include "resource.h"
#include "AIPrefix.h"


#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

//#define CSIDL_MYDOCUMENTS		0x0005 //My Documents (c4005 macro redef)
#define DWL_MSGRESULT				0
#define COL_SELLIM						10
#define MAX_LOADSTRING			255
#define RCDATALIM						2500 //compatible with SIZEOF(bytes)
#define MAX_GUIMOVPERSESS		25
#define MAX_FILE							4194304


//Aspect ratios over 9
#define FOURTHREEVID					12
#define WIDESCREENVID				16
#define UNIVISIUM						18 // e.g. Galaxy Tabs
#define ULTRAWIDEVID					21
#define DESIGNSCRX						1360
#define DESIGNSCRY						768




//Header names
struct ColHeading
{
	std::wstring ColHeadNames[4];
};
struct FileColHeading
{
	std::wstring FileColHeadNames[3];
};

struct ColWidth
{
	int ColWid[4];
};
// Global Variables:
UINT const WM_COPYGLOBALDATA = 0x0049; //Drop files filter
const wchar_t TEMP_CLASS_NAME[14]  = L"ResCheckClass";
typedef std::vector<std::wstring> LVA;
typedef std::vector<LVA> LVA2D;
const std::wstring ColSelectors[COL_SELLIM][MAX_LOADSTRING] = {{L"GameForms"}, {L"Selection2"}, {L"Selection3"}, {L"Selection4"}, {L"Selection5"}, {L"Selection6"}, {L"Selection7"}, {L"Selection8"}, {L"Selection9"}, {L"Selection10"}};
const uint32_t maxDWORD = 32767;
const LONGLONG ID_LIST = 200;
wchar_t const EOL_DELIM = L'\n';
wchar_t const COMMA_DELIM = L',';
int const SIZEOF_WCHAR= sizeof(wchar_t);
//extern int i, retVal; //avoids LNK2005
extern wchar_t const *lPref;
extern int i; //avoids LNK2005
extern int retVal; //avoids LNK2005
extern float resX, resY;
wchar_t const* FormatItowNotify(int a, wchar_t(buf)[16], wchar_t *buffer ); //default argument not allowed here else c2572 redefinition
void * ReallocateMem(wchar_t * aSource, int Size);
BOOL GetFilesIn(HWND LVFileshWnd, wchar_t *currPathW);
BOOL CreateLVItems(HWND hwndList, std::wstring Text1, int i, int j = 0);
BOOL GetResource(HWND LVRepshWnd, int rcName, const wchar_t *rcStrType, const wchar_t *rcStrType1 = NULL,  int rcIntType = 0);
const wchar_t  * LoadInResource(DWORD& size,  int name, const wchar_t *rcStrType, int rcIntType = 0);
LVA2D ResProc(const wchar_t* str, const wchar_t delimiter, int &iMax, int &jMax);
BOOL GetRegVal(wchar_t* keyName, wchar_t* valueName, wchar_t* valueData);
void ErrorExit(LPCWSTR lpszFunction, LPCWSTR var =NULL);
BOOL ChangeWindowMsgFilterEx(HWND hWnd, UINT uMsg);

// Forward declarations of functions included in FuzzyQuasiSKSE.cpp:
ATOM MyRegisterClass(HINSTANCE hInstance, BOOL Ephemeral = FALSE);
HWND InitInstance(HINSTANCE, BOOL Ephemeral = FALSE);
int SwitchResolution (HWND hwndParent, INT_PTR (WINAPI * dProc)(HWND, UINT, WPARAM, LPARAM));
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void DlgResize(HWND OwnerHwnd);
BOOL MoveCtrl(HWND ownerHwnd, float szFactorX, float szFactorY, int ctrlID, HWND hWndID = 0); //resource or HWND?
LRESULT CALLBACK staticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
INT_PTR CALLBACK DialogPage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL LVPRocEvents(HWND LVhWnd, WPARAM wp, const int evt);
float DoSystemParametersInfoStuff(HWND hwnd, bool progLoad =FALSE);
int OnSelChange(int setTab=999);
BOOL InitListView(HWND hwndLV, int lvType);
HWND CreateListView(HINSTANCE hInstance, int lvType, HWND Ownerhwnd);
BOOL CreateOtherCtrls();
BOOL CreateColumn(HWND hwndLV, int iCol, std::wstring Text, int iWidth, BOOL delDummyCol = FALSE);
void AutoSizeCols(HWND LVhWnd);
void Kleenup();
LRESULT ProcessCustomDraw (LPARAM lParam);


//Versioning
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
typedef NTSTATUS(NTAPI *fnRtlGetVersion )(PRTL_OSVERSIONINFOW lpVersionInformation);

#ifdef __cplusplus

#define VERSIONHELPERAPI inline bool

#else  // __cplusplus

#define VERSIONHELPERAPI FORCEINLINE BOOL

#endif // __cplusplus


VERSIONHELPERAPI
IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	/*OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
	DWORDLONG        const dwlConditionMask = VerSetConditionMask(
	VerSetConditionMask(
	VerSetConditionMask(
	0, VER_MAJORVERSION, VER_GREATER_EQUAL),
	VER_MINORVERSION, VER_GREATER_EQUAL),
	VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
	osvi.dwMajorVersion = wMajorVersion;
	osvi.dwMinorVersion = wMinorVersion;
	osvi.wServicePackMajor = wServicePackMajor;
	return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;*/

	RTL_OSVERSIONINFOEXW verInfo = { 0 };
	verInfo.dwOSVersionInfoSize = sizeof( verInfo );

	static auto RtlGetVersion = (fnRtlGetVersion)GetProcAddress( GetModuleHandleW( L"ntdll.dll" ), "RtlGetVersion" );

	if (RtlGetVersion != 0 && RtlGetVersion( (PRTL_OSVERSIONINFOW)&verInfo ) == 0)
	{
		if (verInfo.dwMajorVersion > wMajorVersion)
			return true;
		else if (verInfo.dwMajorVersion < wMajorVersion)
			return false;

		if (verInfo.dwMinorVersion > wMinorVersion)
			return true;
		else if (verInfo.dwMinorVersion < wMinorVersion)
			return false;

		if (verInfo.wServicePackMajor >= wServicePackMajor)
			return true;
	}

	return false;
}


VERSIONHELPERAPI
IsWindowsVistaOrGreater()
{
	return IsWindowsVersionOrGreater( HIBYTE( _WIN32_WINNT_VISTA ), LOBYTE( _WIN32_WINNT_VISTA ), 0 );
}

