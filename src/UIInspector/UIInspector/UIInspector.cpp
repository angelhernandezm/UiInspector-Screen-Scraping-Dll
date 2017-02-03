// UIInspector.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UIInspector.h"

UIINSPECTOR_API HRESULT GetUIElements(const wchar_t* processIdOrName, wchar_t* resultAsXml, int& cbRequired) {
	HWND targetWnd;
	int idCount = 0;
	size_t cbString = 0;
	CUIInspector inspector;
	HRESULT retval = S_FALSE;
	basic_string<wchar_t> xml;
	vector<UIElement> elements;
	wchar_t tempBuffer[MAX_BUFFER_SIZE];

	if ((targetWnd = inspector.GetTargetWindow(processIdOrName)) != NULL) {
		inspector.InspectWindow(elements, targetWnd);

		if (elements.size() > 0) {
			xml.append(L"<?xml version=\"1.0\" encoding=\"utf-8\" ?><uiInspector>");

			std::for_each(elements.begin(), elements.end(), [&] (UIElement element) {
				StringCbPrintfW(tempBuffer, BUFFER_SIZE(tempBuffer), L"<element id=\"%d\" hWnd=\"0x%X\" parentHwnd=\"%d\" className=\"%s\" text=\"%s\"/>",
					++idCount, element.hWnd, element.Parent, element.ClassName.data(), element.Text.data());

				xml.append(tempBuffer);
			});

			xml.append(L"</uiInspector>");

			if (resultAsXml != nullptr && (cbString = wcslen(resultAsXml)) > 0) {
				if (cbString >= xml.size()) {
					cbRequired = 0;
					wcsncpy_s(resultAsXml, cbString, xml.data(), _TRUNCATE);
					retval = S_OK;
				} else {
					cbRequired = xml.size() - cbString;
					wcsncpy_s(resultAsXml, cbString, xml.data(), _TRUNCATE);
				}
			}
		}
	}

	return retval;
}

void UIINSPECTOR_API CUIInspector::InspectWindow(vector<UIElement>& elements, HWND hWnd) {
	::EnumChildWindows(hWnd, &CUIInspector::EnumChildProc, (LPARAM) &elements); 
}

HWND UIINSPECTOR_API CUIInspector::GetTargetWindow(const wchar_t* processIdOrName) {
	int processId;
	HWND retval = NULL;

	if ((retval = ::FindWindow(NULL, processIdOrName)) == NULL) {
		if ((processId = _wtoi(processIdOrName)) > 0) {
			SearchArgs args(processId);
			::EnumWindows(&CUIInspector::EnumWindowProc, (LPARAM) &args);

			if (args.found) 
				retval = args.hWnd;
		}
	}

	return retval;
}


void UIINSPECTOR_API CUIInspector::GetControlOrWindowText(HWND hWnd, wchar_t* text) {
	::SendMessage(hWnd, WM_GETTEXT, MAX_PATH, (LPARAM) text);
}

BOOL UIINSPECTOR_API CUIInspector::EnumChildProc(HWND hWnd, LPARAM lParam) {
	CUIInspector inspector;
	wchar_t className[MAX_PATH], text[MAX_PATH];
	auto items = reinterpret_cast<vector<UIElement>*>(lParam);
	::GetClassName(hWnd, className, wcslen(className));
	inspector.GetControlOrWindowText(hWnd, &text[0]);
	items->push_back(UIElement(basic_string<wchar_t>(text), (int) hWnd, (int) ::GetParent(hWnd), basic_string<wchar_t>(className)));
	lParam = (LPARAM) &items;

	return TRUE;
}


BOOL UIINSPECTOR_API CUIInspector::EnumWindowProc(HWND hWnd, LPARAM lParam) {
	TITLEBARINFO pti;
	DWORD lpdwProcessId;
	pti.cbSize = sizeof(TITLEBARINFO);
	auto selectedProcess = reinterpret_cast<SearchArgs*>(lParam);

	if (selectedProcess->processId != 0) {
		::GetWindowThreadProcessId(hWnd, &lpdwProcessId);

		if (lpdwProcessId == selectedProcess->processId && ::IsWindowVisible(hWnd) && ::GetTitleBarInfo(hWnd, &pti)) {
			selectedProcess->found = TRUE;
			selectedProcess->hWnd = hWnd;
			lParam = (LPARAM) &selectedProcess;
		}
	}

	return TRUE;
}