// FuzzyQuasiSKSE.cpp : Defines the entry point for the application.
//
#include "AFuzzyQuasiSKSE.h"
//#include <unknown.h>
//#include "Achievements.h"


//SKSEPapyrusInterface        * g_papyrus = NULL;
_CrtMemState sOld;
_CrtMemState sNew;
_CrtMemState sDiff;

RECT rcWindow;
int selectedTab = 0, tmp =0, tabJustClcked = 0;
WCHAR szTitle[MAX_LOADSTRING];					// The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];		// the main window class name
WCHAR regKeySSE[RCDATALIM];						// fix RCDATALIM later
WCHAR regKeyWowSSE[RCDATALIM];					// fix RCDATALIM later
WCHAR regValNameSSE[MAX_LOADSTRING];		// value name for above reg values
RECT rcClient = {0};											// resizing controls
wchar_t *tempDest;
wchar_t const *lPref = L"\\\\?\\";
float scrAspect = 0, resX = 0, resY =0, sizefactorX = 1, sizefactorY = 1;
int  tabLV = 0, capCallDlgResize = 0, ctGUIMoveX = 0, ctGUIMoveY = 0, nonClientTabDlgwd, nonClientTabDlght;
int iTmp = 0, wd = 0, wdOrig = 0, htOrig = 0, wdOld = 0, htOld = 0, wdBase = 0, htBase = 0;
int wds[MAX_GUIMOVPERSESS] = {0}, hts[MAX_GUIMOVPERSESS] = {0};
//escKeyPressed may not be need immediately
BOOL escKeyPressed = FALSE, isLoading = TRUE, oneCallAutoSizeCols[MAX_TABS] = { FALSE };
BOOL wdRevert = FALSE, htRevert = FALSE, vmEyefinity=FALSE, noGUIMoves = FALSE;
HINSTANCE appHinstance = 0;							// current instance
HWND hwndFuzzyQuasiSKSE  = 0, LVRepshWnd = 0, LVFileshWnd = 0, DLGhwnd = 0, aboutHwnd = 0, editHwnd = 0;
// Other Controls
HWND hwndCancelButton = 0;
// Drag 'n drop
HDROP hDropInfo;

APP_CLASS::APP_CLASS(void)
{

	//DialogBoxW(hInstance, MAKEINTRESOURCEW(IDD_DIALOG1), hwndFuzzyQuasiSKSE, DlgProc);
	DialogBoxParamW(appHinstance, MAKEINTRESOURCEW(IDD_DIALOG1), nullptr, APP_CLASS::s_DlgProc, reinterpret_cast<LPARAM>(this));

}

#ifdef __cplusplus
extern "C" {
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNUSED(hPrevInstance);
	UNUSED(lpCmdLine);
	UNUSED(nCmdShow);

	ATOM        aReturn = 0;

	// _CrtSetBreakAlloc(215); // 215 was a normal block for detected memory leaks!

	// Initialize global strings
	wchar_t tempText[100] =  {0};
	if (GetEnvironmentVariableW(L"VMEYEFINITY",tempText, _countof(tempText)) > 0)
	{
		vmEyefinity = TRUE;
	}
	else if (GetLastError() == ERROR_ENVVAR_NOT_FOUND)
	{
		vmEyefinity = FALSE;
	}

		//regKeySSE = (wchar_t *)calloc(RCDATALIM, SIZEOF_WCHAR);
		//regValNameSSE = (wchar_t *)calloc(RCDATALIM, SIZEOF_WCHAR);
		LoadStringW(hInstance, IDS_SSEREGKEY, regKeySSE, RCDATALIM);
		LoadStringW(hInstance, IDS_SSEREGKEYWOW, regKeyWowSSE, RCDATALIM);
		LoadStringW(hInstance,IDS_SSEVALNAME, regValNameSSE, MAX_LOADSTRING);
		//pathNameSSE = (wchar_t *)calloc(RCDATALIM, SIZEOF_WCHAR); // C6011 warning unavoidable?
		wchar_t* pathNameSSE = SSEPathMem().Var();
		if (GetRegVal(regKeySSE, regValNameSSE, &*pathNameSSE))
		{
			// Registry error
			Kleenup();
			return 0;
		}
		else  //succeeds
		{
			if (NULL==pathNameSSE[0]) //but no path yet: try 64
			{
				if (GetRegVal(regKeyWowSSE, regValNameSSE, &*pathNameSSE))
				{
					//some bizarro set of circumstances
					int msgboxID = MessageBoxW(0, L"Problem retrieving registry entries for Skyrim Special Edition found! Continue?", L"Registry", MB_YESNO | MB_ICONEXCLAMATION | MB_TOPMOST);
					if (msgboxID == IDNO)
					{
						wcscpy_s(pathNameSSE, 24, L"No PathName");
						Kleenup();
						return 0;
					}
				}
				else
				{
					if (NULL==pathNameSSE[0])
					{
						int msgboxID = MessageBoxW(0, L"Problem retrieving registry entries for Skyrim Special Edition found! Continue?", L"Registry", MB_YESNO);
						if (msgboxID == IDNO)
						{
							wcscpy_s(pathNameSSE, 24, L"No PathName");
							Kleenup();
							return 0;
						}

					}
				}
			}
		}

//Init wild card search now
if (NULL==pathNameSSE[0])
wcscpy_s(pathNameSSE, RCDATALIM, L"No PathName");
else
{
	if (wcscat_s(pathNameSSE, RCDATALIM, L"*"))
	{
		ErrorRep(L"Init FileSearch failed. Long Pathname? \nCannot continue!", 0, 1);
		Kleenup();
	}
}



	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_FuzzyQuasiSKSE, szWindowClass, MAX_LOADSTRING);
	
	
	// Create a new window see https://msdn.microsoft.com/en-us/library/windows/desktop/ff381397(v=vs.85).aspx


	if (aReturn = MyRegisterClass(hInstance, true))
	{
	HWND hWnd =  InitInstance(appHinstance, true); 
	if (hWnd == NULL)
	{
		ErrorRep (L"Cannot create the Rescheck window!!!?", 0, 1);
		Kleenup();
		return 0;
	}
	else
	{
		scrAspect = DoSystemParametersInfoStuff(hWnd, true);
		if (!scrAspect || !SwitchResolution (nullptr, APP_CLASS::s_DlgProc))
		{
			Kleenup();
			ExitKleenup();
			_CrtDumpMemoryLeaks();
			return 0;
		}

	}
	ExitKleenup();
	Kleenup();
	return 0; //never gets here, but suppress C4715 warning
	}
	else
	{
		ErrorRep(L"FuzzyQuasiSKSE could not be registered!");
		Kleenup();
		return aReturn;
	}
}
#ifdef __cplusplus
} //extern
#endif

LRESULT CALLBACK RescheckWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	//temp windowfor res check.
}
float DoSystemParametersInfoStuff(HWND hwnd, bool progLoad)
{

	if (progLoad)
	{
		//experimental
		CONST char logname[100]="\\My Games\\Skyrim Special Edition\\skse4\\achievements.log";
		//gLog.IDebugLog::OpenRelative(CSIDL_MYDOCUMENTS, logname);
		//gLog.OpenRelative(CSIDL_MYDOCUMENTS, logname);
		//_DMESSAGE("achievements");


		//destroy ephemeral window 
		if (!DestroyWindow(hwnd)) ErrorRep(L"Rescheck window cannot be destroyed");
		if (!(UnregisterClassW(TEMP_CLASS_NAME, appHinstance))) ErrorRep(L"Rescheck class failed to unregister");
	}

	if (vmEyefinity)
	{
	resX = 1;
	resY = 1;
	return WIDESCREENVID/9;
	}
	else
	{
	HMONITOR hMon = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO monInfo;
	monInfo.cbSize = sizeof(MONITORINFO);

	if (GetMonitorInfoW(hMon, &monInfo))
	{
		resX = (float)abs(monInfo.rcMonitor.right - monInfo.rcMonitor.left);
		resY = (float)abs(monInfo.rcMonitor.top - monInfo.rcMonitor.bottom);
		tmp = (int)(9 * resX /resY);
		//scale factors
		resX = resX/DESIGNSCRX;
		resY = resY/DESIGNSCRY;
		if (abs(1 - resX) < 0.1)
		resX = 1;
		if (abs(1- resY) < 0.1)
		resY = 1;

		if (tmp <=  FOURTHREEVID)
		tmp = FOURTHREEVID;
		else
		{
			if (tmp <=  WIDESCREENVID)
			tmp = WIDESCREENVID;
			else
			{
				if (tmp <=  UNIVISIUM)
				tmp = UNIVISIUM;
				else
				tmp = ULTRAWIDEVID;
			}
		}
		return (float)tmp/9;
	}
	else
	{
		ErrorRep(L"GetMonitorInfo: Cannot get info.");
	}
	return 0;
	}
}
int SwitchResolution (HWND hwndParent, INT_PTR (WINAPI * dProc)(HWND, UINT, WPARAM, LPARAM))
{
	UNUSED(dProc);
	if (hwndParent) //About dialogue
	{
		return 0; //temp until above sorted
	}
	else 
	{
		ATOM aReturn = 0;
		appHinstance = GetModuleHandleW(NULL); //same as hInstance: use for application hInstance: (okay for exe not for DLL)
		if (aReturn = MyRegisterClass(appHinstance))
		{
			hwndFuzzyQuasiSKSE = InitInstance(appHinstance);

			if (!hwndFuzzyQuasiSKSE)
			{
				ErrorRep(L"Cannot Create App window, Quitting...");
				return 0;
			}


		}
		else
		{
		ErrorRep(L"FuzzyQuasiSKSE could not be registered!");
		return aReturn;
		}


		INITCOMMONCONTROLSEX icex;
		icex.dwICC = ICC_LISTVIEW_CLASSES;
		icex.dwSize = sizeof(icex);
		if (InitCommonControlsEx(&icex) == FALSE)
		{
			ErrorRep(L"Initiation of common controls failed!");
			return 0;
		}
		APP_CLASS wnd;
		//APP_CLASS* wnd =(APP_CLASS*)malloc(sizeof(APP_CLASS));
		//wnd = new APP_CLASS();

		MSG msg;
		// Main message loop:
		//HACCEL hAccelTable = LoadAcceleratorsW(appHinstance, MAKEINTRESOURCEW(IDC_FuzzyQuasiSKSE));
		// if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
		while (GetMessageW(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (DestroyWindow(hwndFuzzyQuasiSKSE)) //Probably not necessary- but...
		{
		//Using the following:  ERROR_CLASS_HAS_WINDOWS :	1412 (0x584) (Class still has open windows.)
		if (!(UnregisterClassW(szWindowClass, appHinstance))) ErrorRep(L"App class failed to unregister!");
		}
		return (int)msg.wParam;
	}

}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance, BOOL Ephemeral)
{
	if (Ephemeral)
	{
	WNDCLASSW ReschkC = { };
	ReschkC.lpfnWndProc   = RescheckWindowProc;
	ReschkC.hInstance     = hInstance;
	ReschkC.lpszClassName = TEMP_CLASS_NAME;
	return RegisterClassW(&ReschkC);
}
else
{
	WNDCLASSEXW RSC_CLASS = { };
	
	if( !GetClassInfoExW( NULL, L"#32770", &RSC_CLASS ))
	{
	ErrorRep (L"Cannot get RSC Class!?");
	return 0;
	}
	//The Dialog class is the default Window Class #32770, already registered,


	RSC_CLASS.cbSize        = sizeof(RSC_CLASS);
	//RSC_CLASS.style          = CS_HREDRAW | CS_VREDRAW;
	//RSC_CLASS.lpfnWndProc    = (WNDPROC)MainWndProc;
	//RSC_CLASS.cbClsExtra     = 0;
	//RSC_CLASS.cbWndExtra     = 0;
	RSC_CLASS.hInstance      = hInstance;
	//RSC_CLASS.hIcon          = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_FuzzyQuasiSKSE));
	//RSC_CLASS.hCursor        = LoadCursorW(nullptr, IDC_ARROW); //https://stackoverflow.com/questions/14363929/vs2012-c-warning-c4005-useheader-macro-redefinition/41101907#41101907
	//RSC_CLASS.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	//RSC_CLASS.lpszMenuName   = MAKEINTRESOURCEW(IDC_FuzzyQuasiSKSE);
	RSC_CLASS.lpszClassName  = (LPCWSTR)szWindowClass; //
	//RSC_CLASS.hIconSm        = LoadIconW(RSC_CLASS.hInstance, MAKEINTRESOURCEW(IDI_SMALL));

    return RegisterClassExW(&RSC_CLASS);
}
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, BOOL Ephemeral)
{
	
	if (Ephemeral)
	{
		return CreateWindowExW(
			0,								// Optional window styles.
			TEMP_CLASS_NAME,						// Window class
			L"Nada",	// Window text
			WS_OVERLAPPEDWINDOW,			// Window style
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,	//Size and position
			NULL,							// Parent window
			NULL,							// Menu
			hInstance,						// Instance handle of owning application (wWinMain)
			NULL							// Additional application data
		);

	}
	else
	{
			return CreateWindowExW(
			0,												// Optional window styles.
			(LPCWSTR)szWindowClass,			// Window class
			szTitle,										// Window text
			WS_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES,		// Window style: CW_USEDEFAULT?
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,	//Size and position: X, Y, Width, Height
			HWND_DESKTOP,						// Parent window
			NULL,										// Menu
			appHinstance,							// Instance handle of owning application (wWinMain)
			NULL											// Additional application data
		);
	}

//   ShowWindow(hwndFuzzyQuasiSKSE, nCmdShow);
//   UpdateWindow(hwndFuzzyQuasiSKSE);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
INT_PTR CALLBACK APP_CLASS::s_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	APP_CLASS *pThis; // our "this" pointer will go here

	if (uMsg == WM_INITDIALOG)
	{
		// Recover the "this" pointer which was passed as the last parameter to the ...Dialog...Param function.
		pThis = reinterpret_cast<APP_CLASS*>(lParam);
		// Put the value in a safe place for future use
		SetWindowLongPtrW(hwnd, DWLP_USER, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		// Recover the "this" pointer from where our WM_INITDIALOG handler stashed it.
		pThis = reinterpret_cast<APP_CLASS*>( GetWindowLongPtrW(hwnd, DWLP_USER));
	}

	if (pThis)
	{

		// Now that we have recovered our "this" pointer, let the member function finish the job.
		return pThis->DlgProc(hwnd, uMsg, wParam, lParam);
	}

	// We don't know what our "this" pointer is, so just do the default thing. Hopefully, we didn't need to customize the behavior yet.
	return FALSE; // returning FALSE means "do the default thing"
}

/*
LRESULT CALLBACK MainWndProc(HWND hwndFuzzyQuasiSKSE, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hwndFuzzyQuasiSKSE, message, wParam, lParam);
}
	switch (message)
	{
	// Bizarro behaviour when CreateWindowW(szWindowClass, jumped to the WM_SHOWWINDOW message loop. Deleting/undeleting WM_CREATE... "fixed"
	case WM_CREATE:
	{
	//SetWindowTextW(hWnd, x);
	}
	break;
	case WM_SHOWWINDOW:
	{
	//	LVtext = (wchar_t *)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
	//	wcscpy_s(LVtext, MAX_LOADSTRING, szTitle);


		//  check files

		//wchar_t * rcText1, *rcText2;
		//rcText1= (wchar_t *)calloc(RCDATALIM, SIZEOF_WCHAR);
		//rcText2 = (wchar_t *)calloc(RCDATALIM, SIZEOF_WCHAR);
		//if (rcText1) free (rcText1);
		//if (rcText2) free (rcText2);
		//if (LVtext) free(LVtext);
		//if (tempText) free(tempText);
//Not great here- WM_CREATE better?
		}
		break;
	case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
		break;
    }
    return 0;
}
*/

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
       {
	   aboutHwnd = hDlg;
	   DlgResize(hDlg);
	   return (INT_PTR)TRUE;
	   }
		break;
   case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


LRESULT CALLBACK staticSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	UNUSED(dwRefData);


	//NMLVDISPINFOW* plvdi;
	//...
//uIdSubclass = https://stackoverflow.com/questions/922442/unique-class-type-id-that-is-safe-and-holds-across-library-boundaries
	//switch (((LPNMHDR)lParam)->code) // instead of LPNMHDR  lpnmh = (LPNMHDR) lParam above;
	switch (uMsg) // instead of LPNMHDR  lpnmh = (LPNMHDR) lParam above;
	{
	case  WM_DROPFILES:
	{
		if (hWnd == LVFileshWnd)
		{
			bool fileExisting;
			DWORD attrBit = 0;
			wchar_t* dropBuf = (wchar_t *)calloc(RCDATALIM, SIZEOF_WCHAR);
			tempDest = (wchar_t *)calloc(RCDATALIM, SIZEOF_WCHAR);
			if ((dropBuf == nullptr) || (tempDest == nullptr))
			{
				ErrorRep (L"Allocation: Something has gone wrong with memory!");
				return 0;
			}		
			// Show files dialog if req.

			int n = 0;
			int count = DragQueryFileW((HDROP)wParam, 0xFFFFFFFF, 0, 0 );
			while ( n < count )
			{
				if (DragQueryFileW ((HDROP)wParam, n, dropBuf, RCDATALIM))
				{

					fileExisting = false;
					//pdest = (int)(wcsrchr( dropBuf, separatorFTA ) - dropBuf + 1);
					attrBit = GetFileAttributesW(dropBuf);
					if (0 != (attrBit & FILE_ATTRIBUTE_DIRECTORY) ) break;
					//wcscpy_s(currPathW, pathLength, dblclkString);
					//wcscat_s(currPathW, pathLength, &dropBuf[pdest]);
					// prepend "\\?\" to the path.
					wcscpy_s(tempDest, RCDATALIM, lPref);
					wcscat_s(tempDest, RCDATALIM, dropBuf);
					//if (CopyFileW(tempDest, currPathW, FALSE))
					{
						//check if entry already exists
						//for (i = folderIndex; i < (j = SendMessageW(hList,LB_GETCOUNT,NULL,NULL)); i++)
						{
							//sendMessageErr = SendMessageW(hList, LB_GETTEXT, i, (LPARAM)currPathW);
							//if (0 == wcscmp(currPathW, &dropBuf[pdest])) fileExisting = true;
						}

						//if (!fileExisting) sendMessageErr = SendDlgItemMessageW(hwnd, IDC_LIST, LB_ADDSTRING, 0, (LPARAM)&dropBuf[pdest]);
					} 
					//else ErrorRep (L"CopyFile: Copy of dragged file error: ", 0);
					n++;
					ProcessCompressedFile(tempDest, 't'); // t tests integrity
					//currPathW[0] = L'\0';
					//doFilesFolders(hwnd);
				}
				else
				{
					ErrorRep(L"DragQuery: Failed.");
					break;
				}
			}
		if (tempDest) free(tempDest);
		if (dropBuf) free(dropBuf);
		}
	}
	break;
	case 	WM_KEYDOWN:
	{
	if(uMsg == 0x1b)
	escKeyPressed=TRUE;
	}
	break;
	case WM_WINDOWPOSCHANGED:
	{
		if (hWnd == LVFileshWnd)
			tabLV = 0;
		if (hWnd == LVRepshWnd)
			tabLV = 2;

			switch (tabLV)
			{
			case 0:
			{
				if (oneCallAutoSizeCols[0])
					return FALSE;

				GetWindowRect(hWnd, &rcWindow);
				wd = (int)(rcWindow.right - rcWindow.left) * resX;
				ListView_SetExtendedListViewStyle(hWnd, LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES);
				if (!oneCallAutoSizeCols[0])
				{
					AutoSizeCols(hWnd, tabLV, floor(wd / 3));
					oneCallAutoSizeCols[0] = TRUE;
				}
				
				//UpdateWindow(hWnd);
			}
			break;
			case 1:
			{
			}
			break;
			case 2:
			{
				if (oneCallAutoSizeCols[2])
					return FALSE;
				GetWindowRect(hWnd, &rcWindow);
				wd = (int)(rcWindow.right - rcWindow.left) * resX;
				ListView_SetExtendedListViewStyle(hWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
					if (!oneCallAutoSizeCols[2])
					{
					AutoSizeCols(hWnd, tabLV, floor(wd / 4));
					oneCallAutoSizeCols[2] = TRUE;
					}
				//UpdateWindow(hWnd);
			}
			break;
			default:
				return DefSubclassProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	break;
	case LVN_GETDISPINFOW: // info on callback items.
	{
		/*
		plvdi = (NMLVDISPINFOW*)lParam;

		switch (plvdi->item.iSubItem)
		{
			// set these up for array of names: https://msdn.microsoft.com/en-us/library/windows/desktop/hh298346%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
		case 0:
			//plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szKind;
			break;

		case 1:
			//plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szBreed;
			break;

		case 2:
			//plvdi->item.pszText = rgPetInfo[plvdi->item.iItem].szPrice;
			break;
		default:
			break;
		}
		*/
	}
	break;
	/*
	case LVM_DELETEALLITEMS:
		break;
	case LVM_INSERTCOLUMN:
		break;
		*/

	case LVN_ODCACHEHINT:
	{
		LPNMLVCACHEHINT   lpCacheHint = (LPNMLVCACHEHINT)lParam;
		/*
		Sent when ListView is about to ask for a range of items. On this notification,
		you should load the specified items into your local cache. It is still possible
		to get an LVN_GETDISPINFO for an item that has not been cached,	therefore,
		your application must take into account the chance of this occurring.
		*/
	}
	break;
	case LVN_ODFINDITEMW:
	{
		LPNMLVFINDITEMW lpFindItem = (LPNMLVFINDITEMW)lParam;
		/*
		Sent when the ListView needs a particular item. Return -1 if the item is not found.
		*/
	}
	break;
	// Process LVN_ENDLABELEDIT to change item labels after in-place editing.
	case LVN_BEGINLABELEDITW:
		//return Main_OnEndLabelEdit((LV_DISPINFO *)lParam);
		break;
	case LVN_ENDLABELEDITW:
		//return Main_OnEndLabelEdit((LV_DISPINFO *)lParam);
		break;
		// Process LVN_COLUMNCLICK to sort items by column.
	case LVN_COLUMNCLICK:
		break;
	case NM_DBLCLK:
		break;
	case NM_CLICK:
		break;
	case WM_NCDESTROY:
		// NOTE: this requirement is NOT stated in the documentation, but it is stated in Raymond Chen's blog article...
		RemoveWindowSubclass(hWnd, staticSubClass, uIdSubclass);
		break;
		default:
		break;
	}
	
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK DialogPage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{

	switch(msg)
	{
	case WM_INITDIALOG:
	{
		// system assigns the default keyboard focus only if the dialog box procedure returns TRUE
	}
	break;
	case WM_COMMAND:
	{
		switch (wp)
		{
			case IDM_CANCEL:
		{
				EndDialog(DLGhwnd, wp);
				return 0;
		}
		break;
		}
	}
	break;
	case WM_NOTIFY:
	{
		if ((((LPNMHDR)lp)->hwndFrom) == LVRepshWnd)
		{
			LPNMLISTVIEW pnm = (LPNMLISTVIEW)lp;
			if(pnm->hdr.code == NM_CUSTOMDRAW)
			{
				SetWindowLongW(DLGhwnd, DWL_MSGRESULT, (LONG)ProcessCustomDraw(lp));
				return 1;
			}

			if (LVPRocEvents(LVRepshWnd, wp, ((LPNMHDR)lp)->code))
				return 0;
			else
				return 1;

		}
		else
		{
		if ((((LPNMHDR)lp)->hwndFrom) == LVFileshWnd)
		{
			LPNMLISTVIEW pnm = (LPNMLISTVIEW)lp;
			if(pnm->hdr.code == NM_CUSTOMDRAW)
			{
				SetWindowLongW(DLGhwnd, DWL_MSGRESULT, (LONG)ProcessCustomDraw(lp));
				return 1;
			}

			if (LVPRocEvents(LVFileshWnd, wp, ((LPNMHDR)lp)->code))
				return 0;
			else
				return 1;
		}
		}
		return 1; //successfully processed msg
	}
	break;
	case WM_SIZE: // will affect all tabs
	{
		tabJustClcked =0;
		// *Order* of control moves...
		if(hwnd == tabData.page1)
		{
			switch (selectedTab)
			{
			case 0:
			{
				oneCallAutoSizeCols[0] = FALSE;
				tabJustClcked +=1;
				if (tabJustClcked ==1)
				MoveCtrl(tabData.page2, sizefactorX, sizefactorY, 0, hwndCancelButton);

				GetClientRect(tabData.page1, &rcClient);
				rcClient.top += 25;
				SetWindowPos(LVFileshWnd, HWND_TOPMOST, rcClient.left,  rcClient.top, rcClient.right - rcClient.left,  rcClient.bottom - rcClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				//Using page1 in GetClientRect(tabData.page1, &rcClient) circumvents "bug?" when page3 rcClient is not updated
				SetWindowPos(LVRepshWnd, HWND_NOTOPMOST, rcClient.left,  rcClient.top, rcClient.right - rcClient.left,  rcClient.bottom - rcClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_FRAMECHANGED);
				OnSelChange(0);
			}
			break;
			case 1:
			{

			}
			break;
			case 2:
			{
				oneCallAutoSizeCols[2] = FALSE;
				GetClientRect(tabData.page1, &rcClient);
				rcClient.top += 25;
				SetWindowPos(LVFileshWnd, HWND_TOPMOST, rcClient.left,  rcClient.top ,rcClient.right - rcClient.left,  rcClient.bottom - rcClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
			default:
			break;
			}

		}
		else
		{
			if(hwnd == tabData.page2)
			{
				switch (selectedTab)
				{
				case 0:
				{

				}
				break;
				case 1:
				{
					if (!isLoading)
					{
						GetClientRect(tabData.page3, &rcClient);
						rcClient.top += 25;
						SetWindowPos(LVRepshWnd, HWND_TOPMOST, rcClient.left,  rcClient.top ,rcClient.right - rcClient.left,  rcClient.bottom - rcClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						GetClientRect(tabData.page1, &rcClient);
						rcClient.top += 25;
						SetWindowPos(LVFileshWnd, HWND_TOPMOST, rcClient.left,  rcClient.top ,rcClient.right - rcClient.left,  rcClient.bottom - rcClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					tabJustClcked +=1;
					if (tabJustClcked ==1)
					MoveCtrl(tabData.page2, sizefactorX, sizefactorY, 0, hwndCancelButton);
					OnSelChange(1);
				}
				break;
				case 2:
				{
				}
				break;
				default:
				break;
				}
			}
			else //tabData.page3
			{
				switch (selectedTab)
				{
				case 0:
				{
					if (isLoading)
					{
					GetClientRect(tabData.page3, &rcClient);
					rcClient.top += 25;
					SetWindowPos(LVRepshWnd, HWND_TOPMOST, rcClient.left,  rcClient.top ,rcClient.right - rcClient.left,  rcClient.bottom - rcClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}
				break;
				case 1:
				{

				}
				break;
				case 2:
				{
					if (!isLoading)
					{
						tabJustClcked +=1;
						if (tabJustClcked ==1)
						MoveCtrl(tabData.page2, sizefactorX, sizefactorY, 0, hwndCancelButton);
						GetClientRect(tabData.page3, &rcClient);
						rcClient.top += 25;
						SetWindowPos(LVRepshWnd, HWND_TOPMOST, rcClient.left,  rcClient.top ,rcClient.right - rcClient.left,  rcClient.bottom - rcClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						OnSelChange(2);
					}
				}
				break;
				default:
				break;
				}
			}
		}
		//return 0; //SIZE successful
	}
	break;

	case  WM_DROPFILES:
	{
		//ErrorRep(L"Test"); // dragin registered for Tab.
	}
	break;
	case WM_SHOWWINDOW:
	{
		if ((hwnd == tabData.page1))
		{
			if (selectedTab==0)
			{
			ShowWindow(LVRepshWnd, SW_SHOW);
			ShowWindow(LVFileshWnd, SW_SHOW);
			ShowWindow(tabData.page1, SW_SHOW);
			}
		}
		else //Not happening
		{ 
			if (selectedTab==1)
			{
			}
			else //selectedTab==2
			{
				ShowWindow(LVFileshWnd, SW_SHOW);
				ShowWindow(LVRepshWnd, SW_SHOW);
				ShowWindow(tabData.page1, SW_SHOW);
			}
		}
		break;



	}
	default:
	return FALSE;
	break;
	}


return FALSE;
}

BOOL LVPRocEvents(HWND LVhWnd, WPARAM wp, const int evt)
{
	// may require special treatment for LVRepshWnd & LVFileshWnd 
	UNUSED(wp);

	//LVtext = (wchar_t *)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
	//wcscpy_s(LVtext, MAX_LOADSTRING - 1, L"Love it\0");
	LVITEMW lvi = { 0 };
	int iSlected;
	switch(evt)
	{
	case NM_DBLCLK:
	{
	iSlected=(int)SendMessageW(LVhWnd,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
		if(iSlected==-1)
		{
			//EndDialog(DLGhwnd, wp);
			ErrorRep(L"No Items in ListView!\0");
		}
		else
		{
		wchar_t* LVtext = LVTextMem().Var();
		wcscpy_s(LVtext, MAX_LOADSTRING - 1, szTitle);
		lvi.mask=LVIF_TEXT;
		lvi.iSubItem=0;
		lvi.pszText=LVtext;
		lvi.cchTextMax=256;
		lvi.iItem=iSlected;
		}
		/* 
		SendMessageW(LVRepshWnd,LVM_GETITEMTEXTW, iSlected, (LPARAM)&lvi);

		sprintf(Temp1,Text);

		for(int j=1;j<=5;j++)
		{
		lvi.iSubItem=j;
		SendMessageW(LVRepshWnd,LVM_GETITEMTEXTW, iSlected, (LPARAM)&lvi);
		sprintf(Temp," %s",Text);
		lstrcat(Temp1,Temp);
		}

		MessageBox(LVRepshWnd,Temp1,"test",MB_OK);
		*/
	}
	break;
	case NM_CLICK:
	{
		iSlected=(int)SendMessageW(LVhWnd,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
		//ListView_SetItemState(LVRepshWnd, iSlected, LVIS_SELECTED, LVIS_SELECTED)
		if(iSlected==-1)
		{                      
			LVTextMem().ReleaseVar();
			return TRUE;
		}
	}
	break;
	case LVN_BEGINLABELEDITW: 
	{
	editHwnd=ListView_GetEditControl(LVhWnd);
	GetWindowTextW(editHwnd, LVTextMem().Var(), MAX_LOADSTRING - 1);
	LVTextMem().ReleaseVar();
	}
	break;
	case LVN_ENDLABELEDITW:
	{
		i=(int)SendMessageW(LVhWnd,LVM_GETNEXTITEM,-1,LVNI_FOCUSED);
		if (i == -1)
		{
			LVTextMem().ReleaseVar();
			return TRUE;
		}
		lvi.iSubItem=0;
		if(escKeyPressed) // escape defaults to WM_DESTROY in dialogex so maybe useless
		{
			lvi.pszText=NULL; //Wrong: check this
			SendMessageW(LVhWnd,LVM_SETITEMTEXTW,(WPARAM)i,(LPARAM)&lvi);
			escKeyPressed = FALSE;
		}
		else
		{
			lvi.pszText= LVTextMem().Var();
			editHwnd=ListView_GetEditControl(LVhWnd);
			GetWindowTextW(editHwnd, LVTextMem().Var(), MAX_LOADSTRING - 1);
			SendMessageW(LVhWnd,LVM_SETITEMTEXTW,(WPARAM)i,(LPARAM)&lvi);
		}
		LVTextMem().ReleaseVar();
	}
	break;
	default:
	{
	return FALSE;
	}
}

	return FALSE;
}

INT_PTR  APP_CLASS::DlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{

	DLGhwnd = hwnd;
	switch (msg)
	{
	case WM_INITDIALOG:
	{

		if (IsWindowsVistaOrGreater())
		{
			if (!(ChangeWindowMsgFilterEx(hwnd, WM_DROPFILES) && ChangeWindowMsgFilterEx(hwnd, WM_COPYDATA) && ChangeWindowMsgFilterEx(hwnd, WM_COPYGLOBALDATA)))
			{
				ErrorRep(L"ChangeWindowMsgFilterEx: Could not allow message.");
			}
		}

		// Note the load order of the following - viz WM_SIZE issues -see TabCtrl_AdjustRect below.
		tabData.page1 = CreateDialogW(appHinstance, MAKEINTRESOURCEW(IDD_Page1), DLGhwnd, DialogPage);
		tabData.page3 = CreateDialogW(appHinstance, MAKEINTRESOURCEW(IDD_Page3), DLGhwnd, DialogPage);
		tabData.page2 = CreateDialogW(appHinstance, MAKEINTRESOURCEW(IDD_Page2), DLGhwnd, DialogPage);

		tabData.tab = GetDlgItem(DLGhwnd, IDC_TAB1);

		if (tabData.tab)
		{
			// Select styles here
			if (SetWindowLongPtrW(tabData.tab, GWL_STYLE, GetWindowLongPtrW(tabData.tab, GWL_STYLE) | TCS_BUTTONS | TCS_FLATBUTTONS))
			{
				// ERROR_INVALID_HANDLE here on success, so reset at end of WM_INITDIALOG
			}
			else
			ErrorRep(L"Cannot Set Tab Styles!");

			TabCtrl_SetExtendedStyle(tabData.tab, TCS_EX_FLATSEPARATORS); // TCS_EX_REGISTERDROP for drag & drop

			wchar_t *TAB1_NAME, *TAB2_NAME, *TAB3_NAME;

			TAB1_NAME= (wchar_t *)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
			TAB2_NAME = (wchar_t *)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
			TAB3_NAME = (wchar_t *)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
			wcscpy_s(TAB1_NAME, MAX_LOADSTRING,  L"Tab1");
			wcscpy_s(TAB2_NAME, MAX_LOADSTRING,  L"Tab2");
			wcscpy_s(TAB3_NAME, MAX_LOADSTRING,  L"Tab3");

			TCITEMW tci = { 0 };
			tci.mask = TCIF_TEXT;
			tci.pszText = TAB1_NAME; // L"Tab1"
			SendMessageW(tabData.tab, TCM_INSERTITEMW, 0, (LPARAM)&tci);
			tci.pszText = TAB2_NAME;
			SendMessageW(tabData.tab, TCM_INSERTITEMW, 1, (LPARAM)&tci);
			tci.pszText = TAB3_NAME;
			SendMessageW(tabData.tab, TCM_INSERTITEMW, 2, (LPARAM)&tci);
			if (TAB1_NAME) free (TAB1_NAME);
			if (TAB2_NAME) free (TAB2_NAME);
			if (TAB3_NAME) free (TAB3_NAME);
		}
		else
		{
			ErrorRep(L"Cannot Create Tabs!");
			EndDialog(DLGhwnd, wp);
		}

		if ((LVFileshWnd = CreateListView(appHinstance, SKSEINFO, tabData.page1)))
		{
			DragAcceptFiles(LVFileshWnd, TRUE);
		}
		else
		{
			ErrorRep(L"Cannot create Listview!");
			EndDialog(DLGhwnd, wp);
		}

		if (!(LVRepshWnd = CreateListView(appHinstance,SKSEFILES, tabData.page3)))
		{
			ErrorRep(L"Cannot create Listview!");
			EndDialog(DLGhwnd, wp);
		}
	
		if (!CreateOtherCtrls())
		{
			ErrorRep(L"Cannot create controls!");
			EndDialog(DLGhwnd, wp);
		}

		if (!GetResource(LVRepshWnd, TESFORMS1, L"GAMEFORMS_1",  L"GAMEFORMS_2"))
		{
			ErrorRep(L"Could not locate resource!", NULL, TESFORMS1);
			EndDialog(DLGhwnd, wp);
		}

		//GetFilesIn(LVFileshWnd, SSEPathMem().Var());
		DlgResize(DLGhwnd);
		_CrtMemCheckpoint(&sOld); //take a snapchot	
	}
	break;
	//case WM_CREATE: Create messages handled internally for *Non Dialog forms*
	case WM_CHAR: 
		switch (wp) 
		{ 
		case 0x1B:
			return 0;
		}
		break;
	case WM_NOTIFY:
	{

		{

			if (((LPNMHDR)lp)->code == (UINT)TCN_SELCHANGE)
			{
			selectedTab = OnSelChange();
			}
			else
			{
				if (((LPNMHDR)lp)->code == (UINT)TCN_SELCHANGING)
				{
					return FALSE;
				}
				else
				{
				//return DefWindowProcW(DLGhwnd, msg, wp, lp);
				}
			}
		}
	return 1; //Mainly ignored. Successfully processed msg
	}
	break;
	case WM_SIZING:
	{
		if (wdRevert || htRevert)
		capCallDlgResize = 5;
		else
		{
			if (capCallDlgResize > 10)
			{
				retVal = SetTimer(hwnd,             // handle to main window 
					IDT_DRAGWINDOW,                   // timer identifier 
					10,                           // millisecond interval 
					(TIMERPROC)NULL);               // no timer callback 

				if (retVal == 0)
				{
					ErrorRep(L"No timer is available.");
				}
			}
			else
			capCallDlgResize++;
		}
		return TRUE;
	}
	break;
	case WM_TIMER:
	{
		if (wp == IDT_DRAGWINDOW)
		{
			if (!DlgResize(DLGhwnd, TRUE))
				//fromTimer = TRUE;
			{
				KillTimer(hwnd, IDT_DRAGWINDOW);
				capCallDlgResize = 0;
				DlgResize(DLGhwnd);
			}
		}
	}
	break;
	case WM_EXITSIZEMOVE:
	{
		tabJustClcked = 0;
		if (!isLoading && capCallDlgResize > 4)
		{
			KillTimer(hwnd, IDT_DRAGWINDOW);
			capCallDlgResize = 0;
			DlgResize(DLGhwnd);
		}
		else
			// Stopgap- else DlgResize is never called
			capCallDlgResize = 4;
	}
	return 0;
	break;
	/* Just for interest, for modifying interactive moving or sizing of a window,
	case WM_NCHITTEST:
	{
		LRESULT r = DefWindowProc(hwnd, msg, wp, lp);
		if (r == HTCAPTION)
			r = HTNOWHERE;
		return r;
	}
	*/
	case WM_SHOWWINDOW:
	{
		_CrtMemCheckpoint(&sNew); //take a snapchot 
		if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
		{
			OutputDebugStringW(L"-----------_CrtMemDumpStatistics ---------");
			_CrtMemDumpStatistics(&sDiff);
			OutputDebugStringW(L"-----------_CrtMemDumpAllObjectsSince ---------");
			_CrtMemDumpAllObjectsSince(&sOld);
			OutputDebugStringW(L"-----------_CrtDumpMemoryLeaks ---------");
			_CrtDumpMemoryLeaks();
		}
		isLoading = FALSE;
		// system assigns the default keyboard focus only if the dialog box procedure returns TRUE
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(DLGhwnd, &ps);
		if (hdc)
		{
			// TODO: Add any drawing code that uses hdc here...
			//eg StretchBlt(hdc, 0, 0, rt.right, rt.bottom, hdcMemBitmap, 0, 0, width, height, SRCCOPY);
			EndPaint(DLGhwnd, &ps);
		}
		else
		{
			ErrorRep(L"No display device context is available!");
		}
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wp);
		// Parse the menu selections etc.:
		switch (wmId)
		{
		case IDM_ABOUT:
			{
			DialogBoxW(appHinstance, MAKEINTRESOURCEW(IDD_ABOUTBOX), DLGhwnd, About);
			}
		break;
		case IDM_EXIT:
			{
			//DestroyWindow(DLGhwnd);
			EndDialog(DLGhwnd, wp);
			//return (INT_PTR)TRUE;
			}
		break;
		case IDOK:
		{
			//DestroyWindow(DLGhwnd);
			EndDialog(DLGhwnd, wp);
			//return (INT_PTR)TRUE;
		}
		break;
		case WM_DESTROY:  //=ID_CANCEL):
		{
			//DestroyWindow(DLGhwnd);
			EndDialog(DLGhwnd, wp);
			//escKeyPressed = TRUE;
			return (INT_PTR)TRUE;
		}
		break;
		case WM_CLOSE:
		{
			EndDialog(DLGhwnd, wp);
		}
		break;
		//case IDCANCEL:  EndDialog(DLGhwnd, wp);  break;
		default:	
		return FALSE;
		break;
		}
	break;
	}
	case WM_DESTROY: //=ID_CANCEL
	{
		PostQuitMessage(0);
	}
	break;
	//For Listview see: https://msdn.microsoft.com/en-us/library/windows/desktop/bb774734%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
	default:
	return false;
	}
	return FALSE;
}
BOOL DlgResize(HWND OwnerHwnd, BOOL fromTimer)
{
RECT dlgInPixels;
int ht = 0;

	if (noGUIMoves == TRUE && !fromTimer)
	{
	return TRUE; //requires tooltip
	}

	if (OwnerHwnd == DLGhwnd)
	{
	//Also consider WM_GETMINMAXINFO
	
	GetWindowRect(OwnerHwnd, &rcWindow);




		if (isLoading)
		{
		//RECT dlgInPixels = {60,60,520,300}; // MapDialogRect is actually client portion of GetWindowRect
		//GetWindowRect {0, 0, 718, 552} (then positioned), MapDialogRect{0, 0, 700, 480}, GetClientRect {0, 0, 682, 408}- GetClientRect handles the tabs as well 
		wd = (int)(rcWindow.right - rcWindow.left) * resX;
		ht = (int)(rcWindow.bottom - rcWindow.top) * resY;
		dlgInPixels = {0,0,400,240};
		if (!MapDialogRect(OwnerHwnd, &dlgInPixels))
			ErrorRep(L"MapDialogRect Failed!");
		wdOrig = (int)(dlgInPixels.right - dlgInPixels.left) * resX;
		htOrig =(int)(dlgInPixels.bottom - dlgInPixels.top) * resY;
		rcWindow.left *= resX;
		rcWindow.right *= resX;
		rcWindow.top *= resY;
		rcWindow.bottom *= resY;
		// e.g.: wdOrig may be needed for non client width
		wdBase = wd;
		htBase = ht;
		}
		else
		{
		wd = rcWindow.right - rcWindow.left;
		ht = rcWindow.bottom - rcWindow.top;
			if (wdRevert)
			{
			sizefactorX = 1;
			wdRevert = FALSE;
			}
			if (htRevert)
			{
			sizefactorY = 1;
			htRevert = FALSE;
			}

			//DoSystemParametersInfoStuff?
			if (wd < wdBase)
			{
					if (fromTimer)
					return FALSE;
				wdRevert = TRUE;
				wd =  wdBase;
				rcWindow.right = rcWindow.left +  wd;
			}

			if (ht < (htBase))
			{
				if (fromTimer)
				return FALSE;
			htRevert = TRUE;
			ht = htBase;
			rcWindow.bottom = rcWindow.top + ht;
			}

			if (fromTimer)
			return TRUE;
		}
	



		if	(wdOld == wd)
		{
			if ((sizefactorX < 0.99999) || (sizefactorX > 1.00001))
			sizefactorX = 1;

			if (htOld == ht)
			{
				if ((sizefactorY < 0.99999) || (sizefactorY > 1.00001))
				sizefactorY = 1;
			}
			else
			{
			iTmp = htBase;
				for (i = 0; i<ctGUIMoveY; i++)
				{
					iTmp += hts[i];
				}
			hts[ctGUIMoveY] = ht - iTmp;
			sizefactorY = (float)ht/iTmp;

			ctGUIMoveY +=1;
			}
		}
		else // wdOld <> wd
		{
			if (htOld == ht)
			{
				if ((sizefactorY < 0.99999) || (sizefactorY > 1.00001))
				sizefactorY = 1;
			}
			else
			{
			iTmp = htBase;
				for (i = 0; i<ctGUIMoveY; i++)
				{
				iTmp += hts[i];
				}
			hts[ctGUIMoveY] = ht - iTmp;
			sizefactorY = (float)ht/(float)iTmp;

			ctGUIMoveY +=1;
			}

		iTmp = wdBase;
			for (i = 0; i<ctGUIMoveX; i++)
			{
				iTmp += wds[i];
			}
		wds[ctGUIMoveX] = wd - iTmp;
		sizefactorX = (float)wd/(float)iTmp;

		ctGUIMoveX +=1;
	
	
}

		if ((ctGUIMoveX == MAX_GUIMOVPERSESS) || (ctGUIMoveY == MAX_GUIMOVPERSESS))
		{
		noGUIMoves = TRUE;
		wd = wdBase;
		ht = htBase;
		}


wdOld = wd;
htOld = ht;

//SetWindowPos is problematic here
if (!MoveWindow(OwnerHwnd, rcWindow.left,  rcWindow.top, wd, ht, TRUE))
ErrorRep(L"Problem with MoveWindow!"); 






	//Size Tabs
	GetClientRect(OwnerHwnd, &rcClient);

	//MapWindowPoints(OwnerHwnd, NULL, (LPPOINT)&rcClient,2);

	// Don't scale Top as the tabData.tab protrudes over buttons
	rcClient.top += 25; //Height of Tab: if too small it overlaps Tab Text label
	wd = rcClient.right - rcClient.left;
	ht = rcClient.bottom - rcClient.top + 25;

		if (isLoading)
		{
		//These generally 0
		nonClientTabDlgwd =  wdOrig - wd; //These generally 0
		nonClientTabDlght = htOrig - ht;
		//With setups not supporting DisplayPort the following is required:
		TabCtrl_SetMinTabWidth(tabData.tab, (int)(wd * resX)/10);
		}
		if (wd + nonClientTabDlgwd < wdOrig)
		{
			rcClient.right =rcClient.left +  wdOrig;
			wd =  wdOrig - nonClientTabDlgwd;
		}
		if (ht + nonClientTabDlght < htOrig) 
		{
			rcClient.bottom = rcClient.top + htOrig;
			ht = htOrig - nonClientTabDlght;
		}
	
	// The order of the following statements affects the Clientrec of the LVs in WM_SIZE
	TabCtrl_AdjustRect(tabData.page1,FALSE,&rcClient);
	TabCtrl_AdjustRect(tabData.page3,FALSE,&rcClient);
	TabCtrl_AdjustRect(tabData.page2,FALSE,&rcClient);
		if (!SetWindowPos(tabData.page1, HWND_TOP,  rcClient.left,  rcClient.top, wd, ht, SWP_HIDEWINDOW))
		ErrorRep(L"Problem with SetWindowPos on Tab1!");
		if (!SetWindowPos(tabData.page3, HWND_TOP, rcClient.left,  rcClient.top, wd, ht, SWP_HIDEWINDOW))
		ErrorRep(L"Problem with SetWindowPos on Tab3!"); 
		if (!SetWindowPos(tabData.page2, HWND_TOP, rcClient.left,  rcClient.top, wd, ht, SWP_HIDEWINDOW))
		ErrorRep(L"Problem with SetWindowPos on Tab2!");


		if (wdRevert || htRevert)
		{
			switch (selectedTab)
			{
			case 0:
			{
			//Sleep(30);
			//InvalidateRect(tabData.page1, &rcWindow, FALSE);
			ShowWindow(tabData.page1, SW_SHOW);
			//UpdateWindow(tabData.page1);
			RedrawWindow(OwnerHwnd, NULL, NULL, RDW_UPDATENOW);
			}
			break;
			case 1:
			{
			ShowWindow(tabData.page2, SW_SHOW);
			}
			break;
			case 2:
			{
			ShowWindow(tabData.page3, SW_SHOW);
			}
			break;
			}
		}


	}
	else
	{
		if (OwnerHwnd==aboutHwnd)
		{
		GetWindowRect(OwnerHwnd, &rcWindow);
		SetWindowPos(OwnerHwnd, HWND_TOPMOST, (int)(rcWindow.left * resX),  (int)(rcWindow.top * resY),  (int)(rcWindow.right- rcWindow.left) *  resX, (int)(rcWindow.bottom - rcWindow.top) * resY, SWP_SHOWWINDOW);
		MoveCtrl(OwnerHwnd, 1, 1, IDC_STATIC_ONE);
		MoveCtrl(OwnerHwnd, 1, 1, IDC_STATIC_TWO);
		MoveCtrl(OwnerHwnd, 1, 1, IDC_STATIC_THREE);
		MoveCtrl(OwnerHwnd, 1, 1, IDOK);

		}
		else
		{

		}
	}
return FALSE;
}

BOOL MoveCtrl(HWND ownerHwnd, float szFactorX, float szFactorY, int ctrlID, HWND hWndID)
{
	HWND hwndCtrl;
	if (ctrlID)
	hwndCtrl = GetDlgItem(ownerHwnd, ctrlID);
	else
	hwndCtrl = hWndID;

	if (hwndCtrl)
	{
	GetWindowRect(hwndCtrl, &rcClient); //get window rect of control relative to screen
	MapWindowPoints(NULL, ownerHwnd, (LPPOINT)&rcClient,2);
		if (!SetWindowPos(hwndCtrl, HWND_BOTTOM, (int)(rcClient.left) * ((isLoading)? resX: 1) * szFactorX,  (int)(rcClient.top) * ((isLoading)? resY: 1) * szFactorY, (int)(rcClient.right-rcClient.left) * ((isLoading)? resX: 1) * szFactorX, (int)(rcClient.bottom-rcClient.top) * ((isLoading)? resY: 1) * szFactorY, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE))
		{
			ErrorRep(L"Cannot Move or Resize!", nullptr, ctrlID); //Get many of these so....
			return FALSE;
		}
	
	}
	else
	{
		ErrorRep(L"Cannot Get Control!", nullptr, ctrlID);
		return FALSE;
	}
return TRUE;
}

BOOL CreateOtherCtrls()
{
hwndCancelButton = CreateWindowW(L"BUTTON", L"Cancel", 
	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		100, 100, 70, 25, 
		tabData.page2, (HMENU)IDM_CANCEL, appHinstance, NULL);
if (hwndCancelButton) 
return TRUE;
else
return FALSE;
}

int OnSelChange(int setTab)
{
	int sel =0;
	if (setTab < 999)
	{
		sel = setTab;
	}
	else
	sel = TabCtrl_GetCurSel(tabData.tab);

	ShowWindow(tabData.page1, (sel == 0) ? SW_SHOW : SW_HIDE);
	ShowWindow(tabData.page2, (sel == 1) ? SW_SHOW : SW_HIDE);
	ShowWindow(tabData.page3, (sel == 2) ? SW_SHOW : SW_HIDE);
	return sel;
}


HWND CreateListView(HINSTANCE hInstance, int lvType, HWND Ownerhwnd)
{
	UNUSED(hInstance);
	RECT rcClient;
	GetClientRect(Ownerhwnd, &rcClient);
	DWORD lvFlags = 0;
switch (lvType)
{
case 256: //SKSEFILES
{
	lvFlags = WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS;
}
break;
case 257: //SKSEINFO
{
	lvFlags = WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS;
}
break;
}

	HWND hWndListView = CreateWindowW(WC_LISTVIEWW,
		NULL,
		lvFlags,
		0, 0,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		Ownerhwnd,
		(HMENU)ID_LIST,
		appHinstance,
		NULL);

		// For extended styles see InitListView

	if (!hWndListView)
	{
		ErrorRep(L"Cannot create Listview! Quitting...");
		return NULL;
	}
	if (!SetWindowSubclass(hWndListView, staticSubClass, 1, 0))
	{
		//std::cerr << "Failed to subclass list\n";
		ErrorRep(L"Cannot subclass Listview! Quitting...");
		DestroyWindow(hWndListView);
		return NULL;
	}
	ListView_SetUnicodeFormat(hWndListView, TRUE);
	
	ShowWindow(hWndListView, SW_HIDE);
	return (hWndListView);
}

void Kleenup()
{
	KillTimer(DLGhwnd, IDT_DRAGWINDOW);
	DragAcceptFiles(LVFileshWnd, FALSE);
	SSEPathMem().ReleaseVar();
	//if (pathNameSSE) free(pathNameSSE);
}



LRESULT ProcessCustomDraw (LPARAM lParam)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

	switch(lplvcd->nmcd.dwDrawStage) 
	{
	case CDDS_PREPAINT : //Before the paint cycle begins
						 //request notifications for individual listview items
		return CDRF_NOTIFYITEMDRAW;

	case CDDS_ITEMPREPAINT: //Before an item is drawn
	{
		return CDRF_NOTIFYSUBITEMDRAW;
	}
	break;

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT: //Before a subitem is drawn
	{
		switch(lplvcd->iSubItem)
		{
		case 0:
		{
			lplvcd->clrText   = RGB(255,255,255);
			lplvcd->clrTextBk = RGB(240,55,23);
			return CDRF_NEWFONT;
		}
		break;

		case 1:
		{
			lplvcd->clrText   = RGB(255,255,0);
			lplvcd->clrTextBk = RGB(0,0,0);
			return CDRF_NEWFONT;
		}
		break;  

		case 2:
		{
			lplvcd->clrText   = RGB(20,26,158);
			lplvcd->clrTextBk = RGB(200,200,10);
			return CDRF_NEWFONT;
		}
		break;

		case 3:
		{
			lplvcd->clrText   = RGB(12,15,46);
			lplvcd->clrTextBk = RGB(200,200,200);
			return CDRF_NEWFONT;
		}
		break;

		case 4:
		{
			lplvcd->clrText   = RGB(120,0,128);
			lplvcd->clrTextBk = RGB(20,200,200);
			return CDRF_NEWFONT;
		}
		break;

		case 5:
		{
			lplvcd->clrText   = RGB(255,255,255);
			lplvcd->clrTextBk = RGB(0,0,150);
			return CDRF_NEWFONT;
		}
		break;

		}

	}
	}
	return CDRF_DODEFAULT;
}

//define exports here
#ifdef __cplusplus
extern "C" {
#endif

	
	/*
	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{

		// you need to get the papyrus interface being used by the main skse dll,
		// otherwise skse won't know to ever register your functions
		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);

		// now register the function to register your functions with skse, so skse will
		// know to add your plugins when it is registering all of its own custom functions
		bool btest = g_papyrus->Register(AchievementsNS::RegisterFuncs);

		if (btest)
			_MESSAGE("Register Succeeded"); //example error check

		return true;
	}
	*/
#ifdef __cplusplus
	}
#endif;