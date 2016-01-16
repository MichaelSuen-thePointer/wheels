#ifndef WINMAIN_H
#define WINMAIN_H

#include "WinClass.h"
#include "WinCommDlg.h"
#include <map>

namespace pl
{
namespace windows
{

class WinApplication: public Object
{
	friend class WinControl;
	friend class WinForm;

	using ControlMap = std::map<HWND, WinControl*>;
	using FormList = std::vector<WinForm*>;
protected:
	HINSTANCE instance;
	ControlMap controls;
	WinForm* mainForm;
	FormList forms;

	WinPen::Ptr pen;
	WinBrush::Ptr brush;
	WinFont::Ptr font;

	bool ProcessMessage(bool InMessageLoop);
	bool RegisterForm(WinForm* Form);
	void UnregisterForm(WinForm* Form);
	void RunModal(WinForm* Form);
public:
	WinApplication(HINSTANCE hInstance);

	HINSTANCE GetInstance();
	std::wstring GetAppName();
	void Run();
	void DoEvents();
	WinControl* GetControl(HWND Handle);
	void Terminate();
	WinForm* GetMainForm();

	WinPen::Ptr GetDefaultPen();
	WinBrush::Ptr GetDefaultBrush();
	WinFont::Ptr GetDefaultFont();
};

extern WinClass* GetDefaultClass();
extern WinApplication* GetApplication();

#include "WinMain.inl"
}
}

extern LRESULT CALLBACK SubclassProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
extern LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

#endif // !WINMAIN_H
