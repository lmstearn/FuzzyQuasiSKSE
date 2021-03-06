// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h" //SDK versioning

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif
// Windows specific: C, C++
#include <Windows.h>

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