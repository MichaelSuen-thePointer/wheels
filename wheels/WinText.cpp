#include "WinText.h"
#include <cassert>
namespace pl
{
namespace windows
{

WinEdit::WinEdit(WinContainer* Parent, bool Multiline, bool AlwaysShowSelection)
	: WinControl()
{
	_CreateWindow(0,
				  WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
				  ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT |
				  (AlwaysShowSelection ? ES_NOHIDESEL : 0) |
				  (Multiline ? ES_MULTILINE : 0) | ES_WANTRETURN,
				  WC_EDIT,
				  Parent);
}

WinEdit::~WinEdit()
{
}

LRESULT WinEdit::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_COMMAND_DISPATCHED:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			OnChanged(this);
			break;
		case EN_HSCROLL:
			OnHScroll(this);
			break;
		case EN_VSCROLL:
			OnVScroll(this);
			break;
		case EN_MAXTEXT:
			OnMaxText(this);
			break;
		case EN_UPDATE:
			OnUpdate(this);
			break;
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	case WM_SETTEXT:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		if (GetMultiline())
		{
			OnChanged(this);
		}
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

WinHAlign WinEdit::GetTextAlign()
{
	if (GetStyle(ES_RIGHT))
	{
		return WinHAlign::Right;
	}
	else if (GetStyle(ES_CENTER))
	{
		return WinHAlign::Center;
	}
	else
	{
		return WinHAlign::Left;
	}
}

void WinEdit::SetTextAlign(WinHAlign Align)
{
	SetStyle(ES_LEFT, Align == WinHAlign::Left);
	SetStyle(ES_CENTER, Align == WinHAlign::Center);
	SetStyle(ES_RIGHT, Align == WinHAlign::Right);
}

EditCharDisplay WinEdit::GetCharDisplay()
{
	if (GetStyle(ES_LOWERCASE))
	{
		return EditCharDisplay::LowerCase;
	}
	else if (GetStyle(ES_UPPERCASE))
	{
		return EditCharDisplay::UpperCase;
	}
	else if (GetStyle(ES_NUMBER))
	{
		return EditCharDisplay::DigitOnly;
	}
	else if (GetStyle(ES_PASSWORD))
	{
		return EditCharDisplay::Password;
	}
	else
	{
		return EditCharDisplay::None;
	}
}

void WinEdit::SetCharDisplay(EditCharDisplay Display)
{
	SetStyle(ES_LOWERCASE, Display == EditCharDisplay::LowerCase);
	SetStyle(ES_UPPERCASE, Display == EditCharDisplay::UpperCase);
	SetStyle(ES_NUMBER, Display == EditCharDisplay::DigitOnly);
	SetStyle(ES_PASSWORD, Display == EditCharDisplay::Password);
}

std::wstring WinEdit::GetLine(int Index)
{
	int Length = SendMessageW(_Handle, EM_LINELENGTH, Index, 0);
	wchar_t* Buffer = new wchar_t[Length + 1];
	SendMessageW(_Handle, EM_GETLINE, Index, reinterpret_cast<LPARAM>(Buffer));
	std::wstring Result(Buffer, Length);
	delete[] Buffer;
	return Result;
}

WinStatic::WinStatic(WinContainer* Parent)
	: WinContainer()
{
	_CreateWindow(0,
				  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | SS_LEFT | SS_NOTIFY | SS_NOPREFIX,
				  WC_STATIC,
				  Parent);
}

WinStatic::~WinStatic()
{
}

LRESULT WinStatic::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_COMMAND_DISPATCHED:
		switch (HIWORD(wParam))
		{
		case STN_CLICKED:
			OnClick(this);
			break;
		case STN_DBLCLK:
			OnDblClick(this);
			break;
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

WinHAlign WinStatic::GetTextAlign()
{
	if (GetStyle(SS_RIGHT))
	{
		return WinHAlign::Right;
	}
	else if (GetStyle(SS_CENTER))
	{
		return WinHAlign::Center;
	}
	else
	{
		return WinHAlign::Left;
	}
}

void WinStatic::SetTextAlign(WinHAlign Align)
{
	SetStyle(SS_LEFT, Align == WinHAlign::Left);
	SetStyle(SS_CENTER, Align == WinHAlign::Center);
	SetStyle(SS_RIGHT, Align == WinHAlign::Right);
}

StatusEdge WinStatic::GetBorderStyle()
{
	if (GetStyle(SS_ETCHEDFRAME))
	{
		return StatusEdge::Etched;
	}
	else if (GetStyle(SS_SUNKEN))
	{
		return StatusEdge::Sunken;
	}
	else
	{
		return StatusEdge::None;
	}
}

void WinStatic::SetBorderStyle(StatusEdge Value)
{
	SetStyle(SS_ETCHEDFRAME, Value == StatusEdge::Etched);
	SetStyle(SS_SUNKEN, Value == StatusEdge::Sunken);
}

bool WinListBox::IsMultiSelection()
{
	return GetStyle(LBS_EXTENDEDSEL) || GetStyle(LBS_MULTIPLESEL);
}

WinListBox::WinListBox(WinContainer* Parent, ListBoxStyle Style)
{
	DWORD ListStyle = 0;
	switch (Style)
	{
	case ListBoxStyle::MultiSelection:
		ListStyle = LBS_EXTENDEDSEL;
		break;
	case ListBoxStyle::CheckSelection:
		ListStyle = LBS_MULTIPLESEL;
		break;
	case ListBoxStyle::MultiAndCheck:
		ListStyle = LBS_EXTENDEDSEL | LBS_MULTIPLESEL;
		break;
	}
	_CreateWindow(0,
				  WS_VSCROLL | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
				  LBS_HASSTRINGS | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | ListStyle,
				  WC_LISTBOX,
				  Parent);
}

WinListBox::~WinListBox()
{
}

LRESULT WinListBox::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_COMMAND_DISPATCHED:
		switch (HIWORD(wParam))
		{
		case LBN_DBLCLK:
			OnDblClick(this);
			break;

		case LBN_SELCANCEL:
		case LBN_SELCHANGE:
			OnSelChanged(this);
			break;
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

int WinListBox::GetSelectedIndex()
{
	if (IsMultiSelection() || GetCount() == 0)
	{
		return -1;
	}
	else
	{
		int Result = SendMessageW(_Handle, LB_GETCURSEL, 0, 0);
		return Result == LB_ERR ? -1 : Result;
	}
}

void WinListBox::GetSelectedIndices(std::vector<int>& Indices)
{
	Indices.clear();
	if (IsMultiSelection())
	{
		int Count = GetCount();
		int* Buffer = new int[Count];
		int Result = SendMessageW(_Handle, LB_GETSELITEMS, Count, (LPARAM)Buffer);
		Indices.insert(Indices.end(), Buffer, Buffer + Count);
		delete[] Buffer;
	}
}

std::wstring WinListBox::GetString(int Index)
{
	int Count = SendMessageW(_Handle, LB_GETTEXTLEN, Index, 0);
	if (Count == LB_ERR)
	{
		return L"";
	}
	else
	{
		wchar_t* Buffer = new wchar_t[Count + 1];
		SendMessageW(_Handle, LB_GETTEXT, Index, reinterpret_cast<LPARAM>(Buffer));
		std::wstring Result(Buffer);
		delete[] Buffer;
		return Result;
	}
}

WinComboBox::WinComboBox(WinContainer* Parent, bool ReadOnly)
{
	_CreateWindow(0,
				  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_AUTOHSCROLL | CBS_HASSTRINGS | (ReadOnly ? CBS_DROPDOWNLIST : CBS_DROPDOWN),
				  WC_COMBOBOX,
				  Parent);
}

WinComboBox::~WinComboBox()
{
}

LRESULT WinComboBox::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_COMMAND_DISPATCHED:
		switch (HIWORD(wParam))
		{
		case CBN_CLOSEUP:
			OnCloseUp(this);
			break;
		case CBN_DBLCLK:
			OnDblClick(this);
			break;
		case CBN_DROPDOWN:
			OnDropDown(this);
			break;
		case CBN_EDITCHANGE:
			OnChanged(this);
			break;
		case CBN_EDITUPDATE:
			OnUpdate(this);
			break;
		case CBN_SELCHANGE:
			OnSelChanged(this);
			break;
		case CBN_SELENDCANCEL:
			OnRejectSel(this);
			break;
		case CBN_SELENDOK:
			OnAcceptSel(this);
			break;
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

std::wstring WinComboBox::GetString(int Index)
{
	int Length = SendMessageW(_Handle, CB_GETLBTEXTLEN, Index, 0);
	if (Length == CB_ERR)
	{
		return L"";
	}
	else
	{
		wchar_t* Buffer = new wchar_t[Length + 1];
		SendMessage(_Handle, CB_GETLBTEXT, Index, reinterpret_cast<LPARAM>(Buffer));
		std::wstring Result(Buffer);
		delete[] Buffer;
		return Result;
	}
}

void WinComboBox::SetString(int Index, const std::wstring& String)
{
	int Sel = GetSelectedIndex();
	DeleteString(Index);
	InsertString(Index, String);
	SetSelectedIndex(Sel);
}

void WinComboBox::Clear()
{
	for (int i = GetCount() - 1; i >= 0; i--)
	{
		DeleteString(i);
	}
}

WinStatusItem::WinStatusItem()
	: WinStatusItem(nullptr, 0)
{
}

WinStatusItem::WinStatusItem(WinStatus* Status, int Index)
	: _Status(Status)
	, _Index(Index)
{
}

WinStatusItem::WinStatusItem(const WinStatusItem& Item)
	: WinStatusItem(Item._Status, Item._Index)
{
}

WinStatusItem::~WinStatusItem()
{
}

WinStatusItem& WinStatusItem::operator=(const WinStatusItem& Item)
{
	_Status = Item._Status;
	_Index = Item._Index;
	return *this;
}

WinStatusItem::operator bool()
{
	return _Status != 0 && _Index >= 0 && _Index < _Status->_Items.size();
}

std::wstring WinStatusItem::GetTextLeft()
{
	if (*this)
	{
		return _Status->_Items[_Index].TextLeft;
	}
	else
	{
		return L"";
	}
}

void WinStatusItem::SetTextLeft(const std::wstring& Value)
{
	if (*this)
	{
		_Status->_Items[_Index].TextLeft = Value;
		_Status->RefreshItem(_Index);
	}
}

std::wstring WinStatusItem::GetTextCenter()
{
	if (*this)
	{
		return _Status->_Items[_Index].TextCenter;
	}
	else
	{
		return L"";
	}
}

void WinStatusItem::SetTextCenter(const std::wstring& Value)
{
	if (*this)
	{
		_Status->_Items[_Index].TextCenter = Value;
		_Status->RefreshItem(_Index);
	}
}

std::wstring WinStatusItem::GetTextRight()
{
	if (*this)
	{
		return _Status->_Items[_Index].TextRight;
	}
	else
	{
		return L"";
	}
}

void WinStatusItem::SetTextRight(const std::wstring& Value)
{
	if (*this)
	{
		_Status->_Items[_Index].TextRight = Value;
		_Status->RefreshItem(_Index);
	}
}

int WinStatusItem::GetWidth()
{
	if (*this)
	{
		return _Status->_Items[_Index].Width;
	}
	else
	{
		return 0;
	}
}

void WinStatusItem::SetWidth(int Value)
{
	if (*this && Value >= 0)
	{
		_Status->_Items[_Index].Width = Value;
		_Status->RefreshItems();
	}
}

bool WinStatusItem::GetBorder()
{
	if (*this)
	{
		return _Status->_Items[_Index].Border;
	}
	else
	{
		return false;
	}
}

void WinStatusItem::SetBorder(bool Value)
{
	if (*this)
	{
		_Status->_Items[_Index].Border = Value;
		_Status->RefreshItem(_Index);
	}
}

bool WinStatusItem::GetRaise()
{
	if (*this)
	{
		return _Status->_Items[_Index].Raise;
	}
	else
	{
		return false;
	}
}

void WinStatusItem::SetRaise(bool Value)
{
	if (*this)
	{
		_Status->_Items[_Index].Raise = Value;
		_Status->RefreshItem(_Index);
	}
}

IMPLEMENT_VOID_EVENT(StatusItemEvent, (Sender, Item), (Object* Sender, int Item));

const int STATUS_BAR_MAX_ITEM_COUNT = 256;

WinStatus::ItemRecord::ItemRecord()
	: TextLeft()
	, TextCenter()
	, TextRight()
	, Width(0)
	, Border(true)
	, Raise(false)
{
}

void WinStatus::RefreshItem(int Index)
{
	int wParam = Index;
	if (_Items[Index].Border)
	{
		if (_Items[Index].Raise)
		{
			wParam |= SBT_POPOUT;
		}
	}
	else
	{
		wParam |= SBT_NOBORDERS;
	}
	std::wstring Text = _Items[Index].TextLeft + L'\t' + _Items[Index].TextCenter + L'\t' + _Items[Index].TextRight;
	SendMessage(_Handle, SB_SETTEXT, wParam, reinterpret_cast<LPARAM>(Text.c_str()));
}

void WinStatus::RefreshItems()
{
	int Borders[3];
	SendMessage(_Handle, SB_GETBORDERS, 0, (LPARAM)Borders);
	int w = Borders[0];
	int h = Borders[1];
	int s = Borders[2];
	int Rights[STATUS_BAR_MAX_ITEM_COUNT] = {0};
	int CurrentRight = w;
	for (int i = 0; i < _Items.size(); i++)
	{
		if (i == _Items.size() - 1 && _Items[i].Width == 0)
		{
			Rights[i] = -1;
		}
		else
		{
			CurrentRight += _Items[i].Width;
			Rights[i] = CurrentRight;
			CurrentRight += s;
		}
	}
	SendMessageW(_Handle, SB_SETPARTS, _Items.size(), (LPARAM)Rights);
	for (int i = 0; i < _Items.size(); i++)
	{
		RefreshItem(i);
	}
}

WinStatus::WinStatus(WinContainer* Parent)
{
	_CreateWindow(0,
				  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SBARS_SIZEGRIP,
				  STATUSCLASSNAME,
				  Parent);
	SendMessageW(_Handle, SB_SETUNICODEFORMAT, TRUE, 0);
}

WinStatus::~WinStatus()
{
}

LRESULT WinStatus::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_SIZE:
		CallDefaultProc = false;
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	case WM_NOTIFY_DISPATCHED:
		switch ((reinterpret_cast<NMHDR*>(lParam))->code)
		{
		case NM_CLICK:
		{
			int Index = (reinterpret_cast<NMMOUSE*>(lParam))->dwItemSpec;
			OnClick(this, Index);
			break;
		}
		case NM_DBLCLK:
		{
			int Index = (reinterpret_cast<NMMOUSE*>(lParam))->dwItemSpec;
			OnDblClick(this, Index);
			break;
		}
		case NM_RCLICK:
		{
			int Index = (reinterpret_cast<NMMOUSE*>(lParam))->dwItemSpec;
			OnRightClick(this, Index);
			break;
		}
		case NM_RDBLCLK:
		{
			int Index = (reinterpret_cast<NMMOUSE*>(lParam))->dwItemSpec;
			OnRightDblClick(this, Index);
			break;
		}
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

WinStatusItem WinStatus::AddItem()
{
	return InsertItem(GetItemCount());
}

WinStatusItem WinStatus::InsertItem(int Index)
{
	if (Index >= 0 && Index <= GetItemCount() && GetItemCount() < STATUS_BAR_MAX_ITEM_COUNT)
	{
		ItemRecord Item;
		_Items.insert(_Items.begin() + Index, Item);
		RefreshItems();
		return WinStatusItem(this, Index);
	}
	else
	{
		return WinStatusItem();
	}
}

WinStatusItem WinStatus::GetItem(int Index)
{
	if (Index >= 0 && Index < GetItemCount())
	{
		return WinStatusItem(this, Index);
	}
	else
	{
		return WinStatusItem();
	}
}

int WinStatus::GetItemCount()
{
	return _Items.size();
}

void WinStatus::DeleteItem(int Index)
{
	assert(Index >= 0 && Index < GetItemCount());

	_Items.erase(_Items.begin() + Index);
	RefreshItems();
}

}
}