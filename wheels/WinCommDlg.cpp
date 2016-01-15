#include "WinMain.h"

namespace pl
{
namespace windows
{

UINT_PTR CALLBACK Service_CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
UINT_PTR CALLBACK Service_PFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
UINT_PTR CALLBACK Service_CFHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

WinColorDialog::WinColorDialog()
	: _Handle(0)
	, _Opening(false)
	, Color(RGB(0, 0, 0))
	, CustomColors()
{}

bool WinColorDialog::IsOpening()
{
	return _Opening;
}

bool WinColorDialog::Execute(WinForm* Form)
{
	if (_Opening)
	{
		return false;
	}
	else
	{
		_Opening = true;
		CHOOSECOLOR cc;
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = Form->GetHandle();
		cc.hInstance = NULL;
		cc.rgbResult = Color;
		cc.lpCustColors = CustomColors;
		cc.Flags = CC_ANYCOLOR | CC_ENABLEHOOK;
		cc.lCustData = reinterpret_cast<LPARAM>(this);
		cc.lpfnHook = Service_CCHookProc;
		cc.lpTemplateName = NULL;

		bool OK = ChooseColorW(&cc) != 0;
		Color = cc.rgbResult;
		GetCommdlgService()->UnregisterColorDialog(_Handle);
		_Opening = false;
		_Handle = 0;

		return OK;
	}
}

void WinFileDialog::Initialize()
{
	_FilterBuffer = new wchar_t[_Filter.length() + 2];

	_FilterBuffer[_Filter.length() + 1] = L'\0';
	{
		const wchar_t* Buffer = _Filter.c_str();
		for (size_t i = 0; i < _Filter.length(); i++)
		{
			_FilterBuffer[i] = Buffer[i] == L'|' ? L'\0' : Buffer[i];
		}
	}

	_FileBuffer = new wchar_t[65536];
	{
		wchar_t* Buffer = _FileBuffer;
		for (std::wstring& FileName : FileNames)
		{
			wcscpy_s(Buffer, 65536, FileName.c_str());
			Buffer += FileName.length() + 1;
		}
	}

	_TitleBuffer = new wchar_t[_Title.length() + 1];
	wcscpy_s(_TitleBuffer, _Title.length() + 1, _Title.c_str());

	_InitDirBuffer = new wchar_t[_InitDir.length() + 1];
	wcscpy_s(_InitDirBuffer, _InitDir.length() + 1, _InitDir.c_str());

	_DefExtBuffer = new wchar_t[_DefaultExt.length() + 1];
	wcscpy_s(_DefExtBuffer, _DefaultExt.length() + 1, _DefaultExt.c_str());

	_Handle = 0;
	_Opening = true;
}

void WinFileDialog::Finalize()
{
	_Handle = 0;
	_Opening = false;

	delete[] _FilterBuffer;
	delete[] _FileBuffer;
	delete[] _TitleBuffer;
	delete[] _InitDirBuffer;
	delete[] _DefExtBuffer;
}

void WinFileDialog::FillStruct(OPENFILENAME& Struct, DWORD ExtraFlag)
{
	Struct.lStructSize = sizeof(Struct);
	Struct.hwndOwner = NULL;
	Struct.hInstance = NULL;
	Struct.lpstrFilter = _FilterBuffer;
	Struct.lpstrCustomFilter = NULL;
	Struct.nMaxCustFilter = 0;
	Struct.nFilterIndex = 1;
	Struct.lpstrFile = _FileBuffer;
	Struct.nMaxFile = 65536;
	Struct.lpstrFileTitle = NULL;
	Struct.nMaxFileTitle = 0;
	Struct.lpstrInitialDir = _InitDirBuffer;
	Struct.lpstrTitle = _TitleBuffer;
	Struct.nFileOffset = 0;
	Struct.nFileExtension = 0;
	Struct.lpstrDefExt = _DefExtBuffer;
	Struct.lCustData = reinterpret_cast<LPARAM>(this);
	Struct.lpfnHook = Service_PFNHookProc;
	Struct.lpTemplateName = NULL;
	Struct.pvReserved = NULL;
	Struct.dwReserved = 0;
	Struct.FlagsEx = 0;

	Struct.Flags = OFN_ENABLEHOOK | OFN_ENABLEINCLUDENOTIFY | OFN_EXPLORER | ExtraFlag;
	if (_MultiSelection)	Struct.Flags |= OFN_ALLOWMULTISELECT;
	if (!_AddToRecent)		Struct.Flags |= OFN_DONTADDTORECENT;
	if (_EnableSizing)		Struct.Flags |= OFN_ENABLESIZING;
	if (_ShowHidden)		Struct.Flags |= OFN_FORCESHOWHIDDEN;
	if (!_DereferenceLink)	Struct.Flags |= OFN_NODEREFERENCELINKS;
	if (!_NetworkButton)	Struct.Flags |= OFN_NONETWORKBUTTON;
}

void WinFileDialog::FillResult(OPENFILENAME& Struct)
{
	if (Struct.Flags & OFN_ALLOWMULTISELECT)
	{
		FileNames.clear();
		wchar_t* Buffer = Struct.lpstrFile;
		while (*Buffer)
		{
			int Length = wcslen(Buffer);
			FileNames.push_back(Buffer);
			Buffer += Length + 1;
		}
	}
	else
	{
		FileNames.clear();
		FileNames.push_back(Struct.lpstrFile);
	}
}

WinFileDialog::WinFileDialog()
	: _Handle(0)
	, _Opening(false)
	, _MultiSelection(false)
	, _AddToRecent(false)
	, _EnableSizing(true)
	, _ShowHidden(false)
	, _DereferenceLink(true)
	, _NetworkButton(true)
	, _Filter(L"所有文件(*.*)|*.*")
{}

WinFileDialog::~WinFileDialog()
{}

void WinOpenFileDialog::FillResult(OPENFILENAME& Struct)
{
	WinFileDialog::FillResult(Struct);
	_Readonly = (Struct.Flags & OFN_READONLY) != 0;
}

WinOpenFileDialog::WinOpenFileDialog()
	: _HideReadonly(false)
	, _Readonly(false)
	, _FileMustExist(true)
{
	_Title = L"打开";
}

WinOpenFileDialog::~WinOpenFileDialog()
{}

bool WinOpenFileDialog::Execute(WinForm* Form)
{
	if (_Opening)
	{
		return false;
	}
	DWORD Flags = 0;
	if (_HideReadonly) Flags = OFN_HIDEREADONLY;
	if (_Readonly) Flags |= OFN_READONLY;
	if (_FileMustExist) Flags |= OFN_FILEMUSTEXIST;
	Initialize();

	OPENFILENAME Struct;
	FillStruct(Struct, Flags);
	Struct.hwndOwner = Form->GetHandle();
	bool Result = GetOpenFileNameW(&Struct) != 0;
	GetCommdlgService()->UnregisterFileDialog(_Handle);

	Finalize();
	return Result;
}

WinSaveFileDialog::WinSaveFileDialog()
	: _OverridePrompt(true)
{
	_Title = L"保存";
}

WinSaveFileDialog::~WinSaveFileDialog()
{}

bool WinSaveFileDialog::Execute(WinForm* Form)
{
	if (_Opening)
	{
		return false;
	}
	DWORD Flags = 0;
	if (_OverridePrompt) Flags |= OFN_OVERWRITEPROMPT;
	Initialize();

	OPENFILENAME Struct;
	FillStruct(Struct, Flags);
	Struct.hwndOwner = Form->GetHandle();
	bool Result = GetSaveFileNameW(&Struct) != 0;
	GetCommdlgService()->UnregisterFileDialog(_Handle);
	Finalize();
	return Result;
}

WinFontDialog::WinFontDialog()
	: _Handle(0)
	, _FontInfo()
	, _Opening(false)
	, _TrueTypeOnly(false)
	, _Extended(true)
	, _FontMustExist(true)
	, Font()
	, Color()
{
	Font = GetApplication()->GetDefaultFont();
	_FontInfo = *Font->GetInfo();
	Color = RGB(0, 0, 0);
}

WinFontDialog::~WinFontDialog()
{}

bool WinFontDialog::Execute(WinForm* Form)
{
	if (_Opening)
	{
		return false;
	}
	CHOOSEFONT cf;
	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = Form->GetHandle();
	cf.hDC = NULL;
	cf.lpLogFont = &_FontInfo;
	cf.iPointSize = 0;
	cf.Flags = CF_ENABLEHOOK | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	cf.rgbColors = Color;
	cf.lCustData = reinterpret_cast<LPARAM>(this);
	cf.lpfnHook = Service_CFHookProc;
	cf.lpTemplateName = NULL;
	cf.hInstance = NULL;
	cf.lpszStyle = NULL;
	cf.nFontType = 0;
	cf.nSizeMin = 0;
	cf.nSizeMax = 0;

	if (_TrueTypeOnly) cf.Flags |= CF_TTONLY;
	if (_Extended) cf.Flags |= CF_EFFECTS;
	if (_FontMustExist) cf.Flags |= CF_FORCEFONTEXIST;

	bool Result = ChooseFontW(&cf) != 0;
	if (Result)
	{
		Color = cf.rgbColors;
		Font = MakeShared<WinFont>(&_FontInfo);
	}
	GetCommdlgService()->UnregisterFontDialog(_Handle);
	return Result;
}

WinCommonDialogService::WinCommonDialogService()
	: _MessageColorOnOK(RegisterWindowMessageW(COLOROKSTRINGW))
{}

void WinCommonDialogService::RegisterColorDialog(HWND Handle, WinColorDialog* Dialog)
{
	_ColorDialogs.insert({Handle, Dialog});
	Dialog->_Handle = Handle;
}

void WinCommonDialogService::UnregisterColorDialog(HWND Handle)
{
	_ColorDialogs.erase(Handle);
}

void WinCommonDialogService::RegisterFileDialog(HWND Handle, WinFileDialog* Dialog)
{
	_FileDialogs.insert({Handle, Dialog});
	Dialog->_Handle = Handle;
}

void WinCommonDialogService::UnregisterFileDialog(HWND Handle)
{
	_FileDialogs.erase(Handle);
}

void WinCommonDialogService::RegisterFontDialog(HWND Handle, WinFontDialog* Dialog)
{
	_FontDialogs.insert({Handle, Dialog});
	Dialog->_Handle = Handle;
}

void WinCommonDialogService::UnregisterFontDialog(HWND Handle)
{
	_FontDialogs.erase(Handle);
}

WinCommonDialogService* _CommonDialogService = nullptr;

UINT_PTR CALLBACK Service_CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_INITDIALOG)
	{
		CHOOSECOLOR* pcc = reinterpret_cast<CHOOSECOLOR*>(lParam);
		WinColorDialog* Dialog = reinterpret_cast<WinColorDialog*>(pcc->lCustData);
		_CommonDialogService->RegisterColorDialog(hDlg, Dialog);

		Dialog->OnInit(Dialog);
	}
	else if (uiMsg == _CommonDialogService->_MessageColorOnOK)
	{
		CHOOSECOLOR* pcc = reinterpret_cast<CHOOSECOLOR*>(lParam);
		bool Cancel = false;
		WinColorDialog* Dialog = _CommonDialogService->_ColorDialogs[hDlg];
		Dialog->Color = pcc->rgbResult;
		Dialog->OnOK(Dialog, Cancel);
		return Cancel ? 1 : 0;
	}
	return 0;
}

UINT_PTR CALLBACK Service_PFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_INITDIALOG)
	{
		OPENFILENAME* pofn = reinterpret_cast<OPENFILENAME*>(lParam);
		WinFileDialog* Dialog = reinterpret_cast<WinFileDialog*>(pofn->lCustData);

		_CommonDialogService->RegisterFileDialog(hDlg, Dialog);

		Dialog->OnInit(Dialog);
	}
	else if (uiMsg == WM_NOTIFY)
	{
		OFNOTIFY* pofn = reinterpret_cast<OFNOTIFY*>(lParam);
		if (pofn->hdr.code == CDN_FILEOK)
		{
			bool Cancel = false;
			WinFileDialog* Dialog = reinterpret_cast<WinFileDialog*>(pofn->lpOFN->lCustData);
			Dialog->FillResult(*pofn->lpOFN);
			Dialog->OnOK(Dialog, Cancel);
			if (Cancel)
			{
				SetWindowLongW(hDlg, DWL_MSGRESULT, 1);
				return 1;
			}
		}
	}
	return 0;
}
	
UINT_PTR CALLBACK Service_CFHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_INITDIALOG)
	{
		CHOOSEFONT* pcf = reinterpret_cast<CHOOSEFONT*>(lParam);
		WinFontDialog* Dialog = reinterpret_cast<WinFontDialog*>(pcf->lCustData);
		_CommonDialogService->RegisterFontDialog(hDlg, Dialog);
	}
	return 0;
}

WinCommonDialogService* GetCommdlgService()
{
	return _CommonDialogService;
}

void SetCommdlgService(WinCommonDialogService* Service)
{
	_CommonDialogService = Service;
}

}
}