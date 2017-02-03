// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <Psapi.h>
#include <algorithm>
#include <strsafe.h>

#define MAX_BUFFER_SIZE 0x000200
#define BUFFER_SIZE(x) (MAX_BUFFER_SIZE * sizeof(wchar_t))

using namespace std;