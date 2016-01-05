#include "WinMain.h"

#include <CommCtrl.h>
#include <algorithm>
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\""		\
	"type='win32' "									\
	"name='Microsoft.Windows.Common-Controls' "		\
	"version='6.0.0.0' "							\
	"processorArchitecture='*' "					\
	"publicKeyToken='6595b64144ccf1df' "			\
	"language='*'\""								\
)

namespace pl
{
namespace windows
{

WinClass* _DefaultClass = nullptr;
WinApplication* _Application = nullptr;
WPARAM _TerminatingValue = 0;

/*WinApplication*/

bool WinApplication::ProcessMessage(bool InMessageLoop)
{
	MSG Message;
	bool Process = false;
	bool Result = false;
	if (InMessageLoop)
	{
		switch (GetMessageW(&Message, NULL, 0, 0))
		{
		case 0:
			_TerminatingValue = Message.wParam;
			Process = false;
			Result = false;
			break;
		case -1:
			Process = false;
			Result = true;
			break;
		default:
			Process = true;
			Result = true;
		}
	}
	else
	{
		switch (PeekMessageW(&Message, NULL, 0, 0, PM_NOREMOVE))
		{
		case 0:
			Process = false;
			Result = true;
			break;
		default:
			if (Message.message == WM_QUIT)
			{
				Process = false;
				Result = true;
			}
			else
			{
				PeekMessageW(&Message, NULL, 0, 0, PM_REMOVE);
				Process = true;
				Result = true;
			}
		}
	}
	if (Process)
	{
		AcceleratorManager* Manager = GetAcceleratorManager();
		if (Manager)
		{
			TranslateAcceleratorW(Message.hwnd, Manager->GetHandle(), &Message);
		}
		TranslateMessage(&Message);
		DispatchMessageW(&Message);
	}
	return Result;
}

bool WinApplication::RegisterForm(WinForm* Form)
{
	if (std::find(_Forms.begin(), _Forms.end(), Form) == _Forms.end())
	{
		_Forms.push_back(Form);
	}
	if (_MainForm)
	{
		return false;
	}
	else
	{
		_MainForm = Form;
		return true;
	}
}

void WinApplication::UnregisterForm(WinForm* Form)
{
    auto Place = std::find(_Forms.begin(), _Forms.end(), Form);
	if (Place != _Forms.end())
	{
		_Forms.erase(Place);
	}
}

void WinApplication::RunModal(WinForm* Form)
{
	while (ProcessMessage(true))
	{
		if (Form->GetVisible() == false)
		{
			break;
		}
		Sleep(0);
	}
}

WinApplication::WinApplication(HINSTANCE hInstance)
	: _Instance(hInstance)
	, _Controls()
	, _MainForm(nullptr)
	, _Forms()
	, _Pen(new WinPen(PS_SOLID, 0, RGB(0, 0, 0)))
	, _Brush(new WinBrush(RGB(255, 255, 255)))
	, _Font(new WinFont(L"Î¢ÈíÑÅºÚ", 0, 0, 0, 0, 400, false, false, false, true))
{}

std::wstring WinApplication::GetAppName()
{
	wchar_t Buffer[1024];
	GetModuleFileNameW(_Instance, Buffer, sizeof(Buffer) / sizeof(Buffer[0]));
	return Buffer;
}

void WinApplication::Run()
{
	if (_Forms.size() > 0)
	{
		while (ProcessMessage(true))
		{
			Sleep(0);
		}
	}
}

WinControl* WinApplication::GetControl(HWND Handle)
{
	auto Place = _Controls.find(Handle);
	if (Place != _Controls.end())
	{
		return Place->second;
	}
	return nullptr;
}

void WinApplication::Terminate()
{
    for (auto form : _Forms)
    {
        if (form != _MainForm)
        {
            form->DestroyForm();
        }
    }
    if (_MainForm)
    {
        _MainForm->DestroyForm();
    }
    while (_Forms.size())
    {
        delete *(_Forms.end() - 1);
    }
}

inline
WinClass* GetDefaultClass()
{
	return _DefaultClass;
}

inline
WinApplication* GetApplication()
{
	return _Application;
}

extern void SetCommdlgService(WinCommonDialogService* Service);
extern void InitAccelerator();
extern void DestroyAccelerator();

} //windows
} //pl

using namespace pl;
using namespace pl::windows;

extern void main();

LRESULT CALLBACK SubclassProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	bool CallDefaultProc = true;
	LRESULT Result = 0;
	WinControl* Control = GetApplication()->GetControl(hWnd);

	if (Control)
	{
		Result = Control->ProcessMessage(uiMsg, wParam, lParam, CallDefaultProc);
	}
	if (CallDefaultProc)
	{
		Result = DefSubclassProc(hWnd, uiMsg, wParam, lParam);
	}
	return Result;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	bool CallDefaultProc = true;
	WinControl* Control = GetApplication()->GetControl(hWnd);
	if (Control)
	{
		Result = Control->ProcessMessage(uiMsg, wParam, lParam, CallDefaultProc);
	}
	if (CallDefaultProc)
	{
		Result = DefWindowProcW(hWnd, uiMsg, wParam, lParam);
	}
	return Result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InitCommonControls();
	WinClass DefaultClass(L"TsunClass", false, false, WindowProc, hInstance);
	if (DefaultClass.IsAvailable())
	{
		WinApplication Application(hInstance);
		WinCommonDialogService CommonDialogService;

		_DefaultClass = &DefaultClass;
		_Application = &Application;
		SetCommdlgService(&CommonDialogService);

		InitAccelerator();
		main();
		DestroyAccelerator();
	}
	return _TerminatingValue;
}
