#include "AuxFuncs.h"
Byte srcFile[MAX_FILE] = {};								// file dragged to LV: stack overflow if in function!


wchar_t const* FormatItowNotify(int a, wchar_t(buf)[16], wchar_t *buffer)
{
	// itow_s chops off the terminator, and craps out Free()
	//int lenBuffer = SIZEOF_WCHAR* (wcslen(buffer) + 1);
	//buffer = (wchar_t *)ReallocateMem(buffer, lenBuffer);
	//_itow_s(k, buffer, lenBuffer, 10);

	for (int i = 0; i < 16; ++i) { buf[i] = ' '; }
	int count = _scwprintf(buf);
	// swprintf(buf, count, L"%d", a, L"%-13s", " ");
	swprintf(buf, count,  L"%d", a);
	wcscpy_s(buffer, 16, (wchar_t*)buf);
		if (!buffer)
		{
			ErrorExit(L"Failed to insert Listview item!", (wchar_t*)buf);
		}
		return buf;
}

void * ReallocateMem(wchar_t * aSource, int Size)
{
	wchar_t * buffer = (wchar_t *)realloc(aSource, Size);
	if (buffer)
	{
		buffer[(Size - 2)/ SIZEOF_WCHAR] = '\0';
	}
	else
	{
		wchar_t *buffer = (wchar_t *)calloc(Size - 1, SIZEOF_WCHAR); // retry original size
		//ErrorExit(L"Failed to insert Listview item!", buffer);
	}
	return buffer;
}
void ErrorExit(LPCWSTR lpszFunction, LPCWSTR var)
{
	// To convert int to LPCWSTR:
	// _itow_s( int      _Value, wchar_t* _Buffer, size_t   _BufferCount,radix usually 10);

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

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlenW((wchar_t*)lpMsgBuf) + lstrlenW((wchar_t*)lpszFunction) + lstrlenW((LPCWSTR)var) + 40) * sizeof(WCHAR));

	if (lpDisplayBuf)
	{
		if (dww)
		{
			StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / SIZEOF_WCHAR, L"%s Failed With Error %lu\nExtended information:\n%s", lpszFunction, dww, var, (LPWSTR)lpMsgBuf);
			wprintf(L"\a");  //audible bell
			Beep(400, 500);
			//MessageBeep((UINT) -1); 
			MessageBoxW(nullptr, (LPCWSTR)lpDisplayBuf, L"Error", MB_OK | MB_SYSTEMMODAL);
		}
		else
		{
			StringCchPrintfW((LPWSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / SIZEOF_WCHAR, L"%s: %s", lpszFunction, var, (LPWSTR)lpMsgBuf);
			MessageBoxW(nullptr, (LPCWSTR)lpDisplayBuf, L"Informational", MB_OK | MB_TASKMODAL);
		}

		if (LocalFree(lpDisplayBuf) || LocalFree(lpMsgBuf))
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
		wchar_t * buffer = (wchar_t *)calloc(MAX_LOADSTRING, SIZEOF_WCHAR);
		_itow_s(retVal, buffer, MAX_LOADSTRING, 10);
		ErrorExit(L"Could not open the file!", buffer);
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
					ErrorExit(L"Decompress: LZMA_STATUS_NOT_FINISHED!");
				}
				else
				{
					if (status == LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK)
					{
						retVal = -1;
						ErrorExit(L"Decompress: LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK!");
					}
				}
			}
			break;
			case SZ_ERROR_DATA:
			{
				ErrorExit(L"Decompress data error: SZ_ERROR_DATA!");
			}
			break;
			case SZ_ERROR_MEM:
			{
				ErrorExit(L"Decompress memory allocation error: SZ_ERROR_MEM!");
			}
			break;
			case SZ_ERROR_UNSUPPORTED:
			{
				ErrorExit(L"Decompress unsupported properties: SZ_ERROR_UNSUPPORTED!");
			}
			break;
			case SZ_ERROR_INPUT_EOF:
			{
				ErrorExit(L"Decompress more bytes required in input buffer (src): SZ_ERROR_INPUT_EOF!");
			}
			break;
			}
		}
		delete[] where;
	}

	return 0;
}
