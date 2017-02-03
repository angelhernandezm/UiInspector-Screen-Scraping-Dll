// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UIINSPECTOR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UIINSPECTOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef UIINSPECTOR_EXPORTS
#define UIINSPECTOR_API __declspec(dllexport)
#else
#define UIINSPECTOR_API __declspec(dllimport)
#endif


struct SearchArgs {
	HWND hWnd;
	BOOL found;
	int processId;

	SearchArgs(int Pid) {
		processId = Pid;
	}
};

struct UIElement {
	int hWnd;
	int Parent;
	basic_string<wchar_t> Text;
	basic_string<wchar_t> ClassName;

	UIElement Empty() const {
		return UIElement(L"", 0, 0, L"");
	}

	UIElement(const basic_string<wchar_t> text, const int Hwnd, const int parent, const basic_string<wchar_t> classname) {
		hWnd = Hwnd;
		Text = text;
		Parent = parent;
		ClassName = classname;
	}
};

// This class is exported from the UIInspector.dll
class UIINSPECTOR_API CUIInspector {
public:
	HWND GetTargetWindow(const wchar_t* processIdOrName);
	void GetControlOrWindowText(HWND hWnd, wchar_t* text);
	static BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);
	void InspectWindow(vector<UIElement>& elements, HWND hWnd);
};

extern "C"
{
	UIINSPECTOR_API HRESULT GetUIElements(const wchar_t* processIdOrName, wchar_t* resultAsXml, int& cbRequired);
}
