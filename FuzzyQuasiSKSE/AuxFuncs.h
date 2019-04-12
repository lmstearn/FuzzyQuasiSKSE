#pragma once
#include "AInfoLoad.h"
#define ITOW(A) Itow((A), &(char[16]){ 0 })
wchar_t const* FormatItowNotify(int a, wchar_t(buf)[16], wchar_t *buffer = nullptr);
void * ReallocateMem(wchar_t * aSource, int Size);
void ErrorExit(LPCWSTR lpszFunction, LPCWSTR var);
int ProcessCompressedFile(wchar_t* tempDest, const char command7z);
