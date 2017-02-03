// Tester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>

using namespace std;

typedef HRESULT (*ptrFun) (const wchar_t* processIdOrName, wchar_t* resultAsXml, int& required);

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT retval;
	ptrFun callback;
	
	HINSTANCE hInstance = LoadLibrary(L"C:\\Users\\Angel\\Desktop\\UIInspector\\UIInspector\\Debug\\UIInspector.dll");

	if (hInstance != NULL) {
		if ((callback = (ptrFun) (GetProcAddress(hInstance, "GetUIElements"))) != NULL) {
			int required;
			int elem = 7000;
			wchar_t* buffer = new wchar_t[elem];
			buffer[elem - 1]='\0';
			int x = wcslen(buffer);
			retval = callback(L"iTunes", buffer, required);
			wprintf(L"%s\n", buffer);
			delete [] buffer;
		}
		FreeLibrary(hInstance);
	}

	_getche();
	
	return 0;
}

