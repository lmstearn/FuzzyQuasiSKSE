#pragma once
#include "AInfoLoad.h"
void ErrorExit(LPCWSTR lpszFunction, LPCWSTR var);
int ProcessCompressedFile(wchar_t* tempDest, const char command7z);
