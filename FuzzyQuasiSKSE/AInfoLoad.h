#pragma once
#include "AuxFuncs.h"

// Debugging
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG  
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)  
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else  
#define DBG_NEW new
#define new new
#define DEBUG_CLIENTBLOCK  
#endif // _DEBUG  
//static int nBreaker = _CrtSetBreakAlloc(215);
//static int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	/* Informational: Same is in header
#if (NTDDI_VERSION >= NTDDI_VISTA)
#define LVS_EX_JUSTIFYCOLUMNS   0x00200000  // Icons are lined up in columns that use up the whole view area.
#define LVS_EX_TRANSPARENTBKGND 0x00400000  // Background is painted by the parent via WM_PRINTCLIENT
#define LVS_EX_TRANSPARENTSHADOWTEXT 0x00800000  // Enable shadow text on transparent backgrounds only (useful with bitmaps)
#define LVS_EX_AUTOAUTOARRANGE  0x01000000  // Icons automatically arrange if no icon positions have been set
#define LVS_EX_HEADERINALLVIEWS 0x02000000  // Display column header in all view modes
#define LVS_EX_AUTOCHECKSELECT  0x08000000
#define LVS_EX_AUTOSIZECOLUMNS  0x10000000
#define LVS_EX_COLUMNSNAPPOINTS 0x40000000
#define LVS_EX_COLUMNOVERFLOW   0x80000000
#endif
	*/

static wchar_t* myVar2;

//Header names
typedef struct ColHeading
{
	std::wstring ColHeadNames[4];
}ColHeading;
typedef struct FileColHeading
{
	std::wstring FileColHeadNames[3];
}FileColHeading;

struct ColWidth
{
	int ColWid[4];
};
void ExitKleenup();
BOOL GetFilesIn(HWND LVFileshWnd, wchar_t *currPathW);
BOOL CreateLVItems(HWND hwndList, std::wstring Text1, int i, int j = 0);
BOOL GetResource(HWND LVRepshWnd, int rcName, const wchar_t *rcStrType, const wchar_t *rcStrType1 = NULL,  int rcIntType = 0);
const wchar_t  * LoadInResource(DWORD& size,  int name, const wchar_t *rcStrType, int rcIntType = 0);
LVA2D ResProc(const wchar_t* str, const wchar_t delimiter, int &iMax);
BOOL GetRegVal(wchar_t* keyName, wchar_t* valueName, wchar_t* valueData);
BOOL ChangeWindowMsgFilterEx(HWND hWnd, UINT uMsg);

// Forward declarations of functions included in FuzzyQuasiSKSE.cpp:
ATOM MyRegisterClass(HINSTANCE hInstance, BOOL Ephemeral = FALSE);
HWND InitInstance(HINSTANCE, BOOL Ephemeral = FALSE);
int SwitchResolution (HWND hwndParent, INT_PTR (WINAPI * dProc)(HWND, UINT, WPARAM, LPARAM));
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
BOOL DlgResize(HWND OwnerHwnd, BOOL fromTimer = FALSE);
BOOL MoveCtrl(HWND ownerHwnd, float szFactorX, float szFactorY, int ctrlID, HWND hWndID = 0); //resource or HWND?
LRESULT CALLBACK staticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
INT_PTR CALLBACK DialogPage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL LVPRocEvents(HWND LVhWnd, WPARAM wp, const int evt);
float DoSystemParametersInfoStuff(HWND hwnd, bool progLoad = FALSE);
int OnSelChange(int setTab = 999);
BOOL InitListView(HWND hwndLV, int lvType);
HWND CreateListView(HINSTANCE hInstance, int lvType, HWND Ownerhwnd);
BOOL CreateOtherCtrls();
BOOL CreateColumn(HWND hwndLV, int iCol, std::wstring Text, int iWidth, BOOL delDummyCol = FALSE);
void AutoSizeCols(HWND LVhWnd, int selectedTab, int widFactor = 0);
LRESULT ProcessCustomDraw (LPARAM lParam);
void Kleenup();


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
class X
{
	const int allocSize = RCDATALIM;
public:
	wchar_t* Var()
	{
		if (myVar2)
			return myVar2;
		else
		{
			myVar2 = (wchar_t*)calloc(allocSize, SIZEOF_WCHAR);
			return myVar2;
		}
	}
	~X()
	{
		if (myVar2 && *myVar2 != NULL)
		{
			free(myVar2);
			myVar2 = nullptr;
		}
	}
	void ReleaseVar()
	{
		if (myVar2 && *myVar2 != NULL)
		{
			free(myVar2);
			myVar2 = nullptr;
		}
	}
};

