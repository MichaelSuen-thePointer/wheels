#pragma once
#ifndef WINCOMMDLG_H
#define WINCOMMDLG_H

#include <map>
#include <vector>
#include "WinClass.h"
#include "WinGDI.h"

namespace pl
{
namespace windows
{

class WinColorDialog: public Object
{
	friend class WinCommonDialogService;
protected:
	HWND _Handle;
	bool _Opening;
public:
	COLORREF Color;
	COLORREF CustomColors[16];

	NotifyEvent OnInit;
	QueryEvent OnOK;

	WinColorDialog();

	bool IsOpening();
	bool Execute(WinForm* Form);

};

class WinFileDialog:public Object
{
	friend class WinCommonDialogService;
	friend UINT_PTR CALLBACK Service_PFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND _Handle;
	bool _Opening;

	bool _MultiSelection;
	bool _AddToRecent;
	bool _EnableSizing;
	bool _ShowHidden;
	bool _DereferenceLink;
	bool _NetworkButton;
	std::wstring _Title;
	std::wstring _InitDir;
	std::wstring _Filter;
	std::wstring _DefaultExt;

	PWCHAR _FilterBuffer;
	PWCHAR _FileBuffer;
	PWCHAR _TitleBuffer;
	PWCHAR _InitDirBuffer;
	PWCHAR _DefExtBuffer;

	void Initialize();
	void Finalize();
	void FillStruct(OPENFILENAME& Struct, DWORD ExtraFlag);
	void FillResult(OPENFILENAME& Struct);
public:
	NotifyEvent OnInit;
	QueryEvent OnOK;

	WinFileDialog();
	~WinFileDialog();

	std::vector<std::wstring> FileNames;

	bool GetMultiSelection();
	void SetMultiSelection(bool Value);

	bool GetAddToRecent();
	void SetAddToRecent(bool Value);

	bool GetEnableSizing();
	void SetEnableSizing(bool Value);

	bool GetShowHidden();
	void SetShowHidden(bool Value);

	bool GetDereferenceLink();
	void SetDereferenceLink(bool Value);

	bool GetNetworkButton();
	void SetNetworkButton(bool Value);

	std::wstring GetTitle();
	void SetTitle(const std::wstring& Value);

	std::wstring GetInitDir();
	void SetInitDir(const std::wstring& Value);

	std::wstring GetFilter();
	void SetFilter(const std::wstring& Value);

	std::wstring GetDefaultExt();
	void SetDefaultExt(const std::wstring& Value);
};

class WinOpenFileDialog: public WinFileDialog
{
protected:
	bool _HideReadonly;
	bool _Readonly;
	bool _FileMustExist;

	void FillResult(OPENFILENAME& Struct);
public:
	WinOpenFileDialog();
	~WinOpenFileDialog();

	bool GetHideReadonly();
	void SetHideReadonly(bool Value);

	bool GetReadonly();
	void SetReadonly(bool Value);

	bool GetFileMustExist();
	void SetFileMustExist(bool Value);

	bool IsOpening();
	bool Execute(WinForm* Form);
};

class WinSaveFileDialog: public WinFileDialog
{
protected:
	bool _OverridePrompt;
public:
	WinSaveFileDialog();
	~WinSaveFileDialog();

	bool GetOverridePrompt();
	void SetOverridePrompt(bool Value);

	bool IsOpening();
	bool Execute(WinForm* Form);
};

class WinFontDialog: public Object
{
	friend class WinCommonDialogService;
	friend UINT_PTR CALLBACK Service_CFHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND _Handle;
	LOGFONT _FontInfo;
	bool _Opening;

	bool _TrueTypeOnly;
	bool _Extended;
	bool _FontMustExist;
public:
	WinFontDialog();
	~WinFontDialog();

	WinFont::Ptr Font;
	COLORREF Color;

	bool GetTrueTypeOnly();
	void SetTrueTypeOnly(bool Value);

	bool GetExtended();
	void SetExtended(bool Value);

	bool GetFontMustExist();
	void SetFontMustExist(bool Value);

	bool IsOpening();
	bool Execute(WinForm* Form);
};

class WinCommonDialogService: public Object
{
	using ColorDialogMap = std::map<HWND, WinColorDialog*>;
	using FileDialogMap = std::map<HWND, WinFileDialog*>;
	using FontDialogMap = std::map<HWND, WinFontDialog*>;

	friend UINT_PTR CALLBACK Service_CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	friend UINT_PTR CALLBACK Service_PFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	friend UINT_PTR CALLBACK Service_CFHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
protected:
	UINT _MessageColorOnOK;
	ColorDialogMap _ColorDialogs;
	FileDialogMap _FileDialogs;
	FontDialogMap _FontDialogs;
public:
	WinCommonDialogService();

	void RegisterColorDialog(HWND Handle, WinColorDialog* Dialog);
	void UnregisterColorDialog(HWND Handle);
	void RegisterFontDialog(HWND Handle, WinFontDialog* Dialog);
	void UnregisterFontDialog(HWND Handle);
	void RegisterFileDialog(HWND Handle, WinFileDialog* Dialog);
	void UnregisterFileDialog(HWND Handle);

};

extern WinCommonDialogService* GetCommdlgService();

#include"WinCommDlg.inl"
}
}
#endif // !WINCOMMDLG_H
