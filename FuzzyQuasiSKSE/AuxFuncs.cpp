#include "AuxFuncs.h"
Byte srcFile[MAX_FILE] = {};								// file dragged to LV: stack overflow if in function!


void FormatItowNotify(int a, wchar_t **buffer)
{
	wchar_t errLVMsg[] = L": Failed to insert Listview item.";

	// itow_s chops off the terminator, and craps out Free()
	SizeT lenBuffer = (wcslen(*buffer) + wcslen(errLVMsg) + SIZEOF_WCHAR);
	wchar_t *buffer1 = ReallocateMem(*buffer, lenBuffer);
	if (buffer1)
	{
		wcscat_s(buffer1, lenBuffer + SIZEOF_WCHAR, errLVMsg);
		// test free(buffer1);
		ErrorRep(buffer1, nullptr, a);
		free(buffer1);
	}
	else
	MessageBoxW(nullptr, L"Memory problem in LV item insertion!", L"ListView insertion: Memory", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		//_itow_s(a, buffer, lenBuffer, 10);

	/*
	for (int i = 0; i < 16; ++i) { buf[i] = ' '; }
	int count = _scwprintf(buf);
	// swprintf(buf, count, L"%d", a, L"%-13s", " ");
	swprintf(buf, count,  L"%d", a);
	wcscpy_s(buffer, 16, (wchar_t*)buf);
		if (!buffer)
		{
			//ErrorRep(buf + L"Failed to insert Listview item!", buf);
		}
*/
}

wchar_t* ReallocateMem(wchar_t * aSource, int Size)
{
	//buffer1 = (wchar_t*)realloc(buffer, MAX_LOADSTRING);
	wchar_t * buffer = (wchar_t *)realloc(aSource, Size);
	if (buffer)
	{
		//buffer[(Size - 2)/ SIZEOF_WCHAR] = '\0';
	}
	else
	{
		wchar_t *buffer = (wchar_t *)calloc((SizeT)Size - 1, SIZEOF_WCHAR); // retry original size
		//ErrorExit(L"Failed to insert Listview item!", buffer);
	}
	return buffer;
}

void ErrorRep(LPCWSTR lpszFunction, wchar_t* extraInf, int var)
{
	wchar_t* buffer = (wchar_t*)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
	// To convert int to LPCWSTR:
	// _itow_s( int      _Value, wchar_t* _Buffer, size_t   _BufferCount,radix usually 10);

	//Example
	//wchar_t* tempDest = (wchar_t*)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
	//size_t m = wcslen(tempDest);
		//No good having count (m) in itow_s as MAX_LOADSTRING as random memory may contain more null terminators!
	//_itow_s(k, tempDest, m, 10);
	//ErrorRep(L"Blah", tempDest);
	//if (tempDest) free(tempDest);
	if (buffer)
	{
		if (var == maxInt)
		ErrorExit(lpszFunction, extraInf);
		else
		{
			DWORD dww = GetLastError();
				if (_itow_s(var, buffer, sizeof(buffer), 10))
				MessageBoxW(nullptr, lpszFunction, L"Error and conversion problem", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
				else
				{
					DWORD dww = GetLastError();
					//size_t m = wcslen(tempDest);
					ErrorExit(lpszFunction, extraInf);
				}
		}

		free(buffer);
	}
	else
	{
		MessageBoxW(nullptr, lpszFunction, L"Error and memory allocation problem", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		exit(0);
	}
}
void ErrorExit(LPCWSTR lpszFunction, LPCWSTR var, bool reportVar)
{

		//courtesy https://msdn.microsoft.com/en-us/library/windows/desktop/ms680582(v=vs.85).aspx
		// also see http://stackoverflow.com/questions/35177972/wide-char-version-of-get-last-error/35193301#35193301
	DWORD dww = 0;
	LPVOID lpMsgBuf = 0;
	LPVOID lpDisplayBuf = 0;

	dww = GetLastError();
	if (dww)
	{
		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			dww,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t*)&lpMsgBuf, 0, nullptr);
	}

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, ((lpMsgBuf)?lstrlenW((wchar_t*)lpMsgBuf): (SizeT)0 + lstrlenW((wchar_t*)lpszFunction) + (var)?lstrlenW((LPCWSTR)var): + (SizeT)80) * SIZEOF_WCHAR);
	
	if (lpDisplayBuf)
	{
		if (dww)
		{
			try
			{
				if (reportVar)
					StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf), L"%s Failed With Error %lu\nExtended information reference: %s : %s", lpszFunction, dww, var, (LPWSTR)lpMsgBuf);
				else
					StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf), L"%s Failed With Error %lu\nExtended information reference: %s", lpszFunction, dww, (LPWSTR)lpMsgBuf);
				wprintf(L"\a");  //audible bell
				Beep(400, 500);
				//MessageBeep((UINT) -1); 
				MessageBoxW(nullptr, (LPCWSTR)lpDisplayBuf, L"Error", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
			}
			catch (...)
			{
				MessageBoxW(nullptr, L"ErrorExit Format Error", L"Error", MB_OK | MB_TASKMODAL | MB_ICONERROR);
			}

		}
		else
		{
			//if (StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf), L"%s: %s", lpszFunction, var))
			//try {
				//StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / SIZEOF_WCHAR, L"%s: %s", lpszFunction, var);
				//}
			//catch e
			try
			{
				if (reportVar)
					StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / SIZEOF_WCHAR, L"%s: %s", lpszFunction, var);
				else
					StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / SIZEOF_WCHAR, L"%s", lpszFunction);

				MessageBoxW(nullptr, (LPCWSTR)lpDisplayBuf, L"Informational", MB_OK | MB_TASKMODAL | MB_ICONINFORMATION);
			}
			catch (...)
			{
				MessageBoxW(nullptr, L"ErrorExit Format Error", L"Error", MB_OK | MB_TASKMODAL | MB_ICONERROR);
			}
			

			
		}
		int ww = 0;
			if ((LocalFree(lpDisplayBuf)))
			ww = 1;
			if (lpMsgBuf)
			{
				if (LocalFree(lpMsgBuf))
				ww = 1;
			}
			if (ww)
			MessageBoxW(nullptr, L"ErrorExit memory problem", L"Error", MB_OK | MB_SYSTEMMODAL);

		//ExitProcess(dw);
	}

}

int ProcessCompressedFile(wchar_t* fName, const char command7z)
{
	UNUSED(command7z);
	FILE* f = {};
	if (retVal = _wfopen_s(&f, fName, L"rb"))
	{
		ErrorRep(L"Could not open the file!", 0, retVal);
		return 1;
	}

	else
	{
		fseek(f, 0, SEEK_END);
		size_t size = ftell(f);

		char* where = new char[size];

		rewind(f);
		fread(where, sizeof(char), size, f);
		fclose(f); // Close the file
		//check if first two bytes is 7z
		if (where[0] == '7' && where[1] == 'z')
		{
			//for (unsigned i = 0; i < size; i++)
			//srcFile[i] = reinterpret_cast<Byte>(&f[i]);
			//srcFile[i] = (Byte)where[i];
			char fNameMBCS[MAX_LOADSTRING] = { ' ' };
			wcstombs(fNameMBCS, fName, MAX_LOADSTRING);
			//memmove(argList + 2, argList, MAX_LOADSTRING - 2);
			const char *argList[3] = { "", "t",fNameMBCS};
			Do7zFile(3, argList);



		}
		else
		{
			//For pure LZMA (bz2 or gz: First Determine file size


			Byte dest = {};
			SizeT destLen = 2 * MAX_FILE;  //8 mb dict size
			const Byte src = *srcFile;
			SizeT srcLen = size;
			const Byte propData[5] = {};
			unsigned propSize = LZMA_PROPS_SIZE;
			ELzmaStatus status = LZMA_STATUS_NOT_SPECIFIED;
			retVal = LzmaUncompress(&dest, &destLen, &src, &srcLen, propData, propSize, status);
			switch (retVal)
			{
			case SZ_OK:
			{
				if (status == LZMA_STATUS_NOT_FINISHED)
				{
					retVal = -1;
					ErrorRep(L"Decompress: LZMA_STATUS_NOT_FINISHED!");
				}
				else
				{
					if (status == LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK)
					{
						retVal = -1;
						ErrorRep(L"Decompress: LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK!");
					}
				}
			}
			break;
			case SZ_ERROR_DATA:
			{
				ErrorRep(L"Decompress data error: SZ_ERROR_DATA!");
			}
			break;
			case SZ_ERROR_MEM:
			{
				ErrorRep(L"Decompress memory allocation error: SZ_ERROR_MEM!");
			}
			break;
			case SZ_ERROR_UNSUPPORTED:
			{
				ErrorRep(L"Decompress unsupported properties: SZ_ERROR_UNSUPPORTED!");
			}
			break;
			case SZ_ERROR_INPUT_EOF:
			{
				ErrorRep(L"Decompress more bytes required in input buffer (src): SZ_ERROR_INPUT_EOF!");
			}
			break;
			}
		}
		delete[] where;
	}

	return 0;
}
