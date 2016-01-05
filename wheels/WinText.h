#pragma once
#ifndef WINTEXT_H
#define WINTEXT_H

#include "WinClass.h"
#include "commctrl.h"

namespace pl
{
namespace windows
{

enum class EditCharDisplay
{
	None, LowerCase, UpperCase, DigitOnly, Password
};

class WinEdit: public WinControl
{
public:
	WinEdit(WinContainer* Parent, bool Multiline, bool AlwaysShowSelection = false);
	~WinEdit();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	NotifyEvent OnChanged;
	NotifyEvent OnHScroll;
	NotifyEvent OnVScroll;
	NotifyEvent OnMaxText;
	NotifyEvent OnUpdate;

	WinHAlign GetTextAlign();
	void SetTextAlign(WinHAlign Value);

	bool GetMultiline();
	void SetMultiline(bool Value);

	EditCharDisplay GetCharDisplay();
	void SetCharDisplay(EditCharDisplay Display);

	wchar_t GetPasswordChar();
	void SetPasswordChar(wchar_t Char);

	bool GetHScroll();
	void SetHScroll(bool Value);

	bool GetVScroll();
	void SetVScroll(bool Value);

	bool GetReadonly();
	void SetReadonly(bool Value);

	bool GetWantReturn();
	void SetWantReturn(bool Value);

	bool GetBorder();
	void SetBorder(bool Value);

	int GetTextLimit();
	void SetTextLimit(int Value);

	bool GetModified();
	void SetModified(bool Value);

	std::wstring GetSel();
	void SetSel(const std::wstring& Sel);

	int GetSelStart();
	void SetSelStart(int Value);

	int GetSelLength();
	void SetSelLength(int Value);

	void Select(int Index, int Count);
	void SelectAll();
	void ClearSel();
	void Copy();
	void Cut();
	void Paste();

	bool GetCanUndo();
	int GetCharFromPos(int X, int Y);
	void GetPosFromChar(int Index, int& X, int& Y);
	int GetFirstVisible();
	std::wstring GetLine(int LineIndex);
	int GetLineCount();
	int GetCurrentLine();
	int GetLineIndex(int CharIndex);
	int GetCharIndex(int LineIndex);

	void ScrollToCaret();
	void ClearUndoBuffer();
	void Undo();
};

enum class StatusEdge
{
	None, Sunken, Etched
};

class WinStatic: public WinContainer
{
public:
	WinStatic(WinContainer* Parent);
	~WinStatic();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure);

	NotifyEvent OnClick;
	NotifyEvent OnDblClick;

	WinHAlign GetTextAlign();
	void SetTextAlign(WinHAlign Align);

	StatusEdge GetBorderStyle();
	void SetBorderStyle(StatusEdge Value);
};

enum class ListBoxStyle
{
	Normal,
	MultiSelection,
	CheckSelection,
	MultiAndCheck
};

class WinListBox: public WinControl
{
protected:
	bool IsMultiSelection();
public:
	WinListBox(WinContainer* Parent, ListBoxStyle Style = ListBoxStyle::Normal);
	~WinListBox();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	NotifyEvent OnDblClick;
	NotifyEvent OnSelChanged;

	int GetFocusedIndex();
	void SetFocusedIndex(int Value);

	int GetSelectedIndex();
	void SetSelectedIndex(int Value);

	int GetItemHeight(int Index);
	void SetItemHeight(int Index, int Value);

	int GetFirstVisibleIndex();
	void SetFirstVisibleIndex(int Value);

	bool GetSelected(int Index);
	void SetSelected(int Index, bool Value);

	void SelectAll(bool Selected);
	void GetSelectedIndices(std::vector<int>& Indices);
	void AddString(const std::wstring& String);
	void InsertString(int Index, const std::wstring& String);
	void DeleteString(int Index);
	int GetCount();
	std::wstring GetString(int Index);
	void SetString(int Index, const std::wstring& String);
	void Clear();

	int FindStringPrefix(const std::wstring& String);
	int FindString(const std::wstring& String);
	void SelectPrefix(const std::wstring& String);
};

class WinComboBox: public WinControl
{
public:
	WinComboBox(WinContainer* Parent, bool ReadOnly);
	~WinComboBox();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	NotifyEvent OnCloseUp;
	NotifyEvent OnDblClick;
	NotifyEvent OnDropDown;
	NotifyEvent OnChanged;
	NotifyEvent OnUpdate;
	NotifyEvent OnSelChanged;
	NotifyEvent OnRejectSel;
	NotifyEvent OnAcceptSel;

	bool GetDisabledScroll();
	void SetDisabledScroll(bool Value);

	int GetSelStart();
	void SetSelStart(int Value);

	int GetSelLength();
	void SetSelLength(int Value);

	void Select(int Index, int Length);

	int GetMinVisible();
	void SetMinVisible(int Value);

	void SelectAll();

	void ClearSel();

	int GetSelectedIndex();
	void SetSelectedIndex(int Index);

	int GetItemHeight();
	void SetItemHeight(int Value);

	int GetFirstVisibleIndex();
	void SetFirstVisibleIndex(int Index);

	bool IsOpened();
	void AddString(const std::wstring& String);
	void InsertString(int Index, const std::wstring& String);
	void DeleteString(int Index);
	int GetCount();

	std::wstring GetString(int index);
	void SetString(int Index, const std::wstring& String);

	void Clear();
	int FindStringPrefix(const std::wstring& String);
	int FindString(const std::wstring& String);
	void SelectPrefix(const std::wstring& String);
	void OpenList();
	void CloseList();
};

class WinStatusItem: public Object
{
	friend class WinStatus;
protected:
	WinStatus* _Status;
	int _Index;

	WinStatusItem(WinStatus* Status, int Index);
public:
	WinStatusItem();
	WinStatusItem(const WinStatusItem& Item);
	~WinStatusItem();

	WinStatusItem& operator=(const WinStatusItem& Item);
	operator bool();

	std::wstring GetTextLeft();
	void SetTextLeft(const std::wstring& Value);

	std::wstring GetTextCenter();
	void SetTextCenter(const std::wstring& Value);

	std::wstring GetTextRight();
	void SetTextRight(const std::wstring& Value);

	int GetWidth();
	void SetWidth(int Value);

	bool GetBorder();
	void SetBorder(bool Value);

	bool GetRaise();
	void SetRaise(bool Value);
};

using StatusItemEvent = Event<Object*, int>;

class WinStatus: public WinControl
{
	friend class WinStatusItem;
protected:
	struct ItemRecord
	{
		using list = std::vector<ItemRecord>;

		std::wstring TextLeft;
		std::wstring TextCenter;
		std::wstring TextRight;
		int Width;
		bool Border;
		bool Raise;

		ItemRecord();
	};

	ItemRecord::list _Items;

	void RefreshItem(int Index);
	void RefreshItems();
public:
	WinStatus(WinContainer* Parent);
	~WinStatus();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	StatusItemEvent OnClick;
	StatusItemEvent OnDblClick;
	StatusItemEvent OnRightClick;
	StatusItemEvent OnRightDblClick;

	WinStatusItem AddItem();
	WinStatusItem InsertItem(int Index);
	WinStatusItem GetItem(int Index);
	int GetItemCount();
	void DeleteItem(int Index);
};

#include "WinText.inl"

}
}
#endif // !WINTEXT_H
