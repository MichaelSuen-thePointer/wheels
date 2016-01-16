inline
bool WinEdit::GetMultiline()
{
	return GetStyle(ES_MULTILINE);
}

inline
void WinEdit::SetMultiline(bool Value)
{
	SetStyle(ES_MULTILINE, Value);
}

inline
wchar_t WinEdit::GetPasswordChar()
{
	return static_cast<wchar_t>(SendMessageW(handle, EM_GETPASSWORDCHAR, 0, 0));
}

inline
void WinEdit::SetPasswordChar(wchar_t Char)
{
	SendMessageW(handle, EM_SETPASSWORDCHAR, Char, 0);
}

inline
bool WinEdit::GetHScroll()
{
	return GetStyle(WS_HSCROLL);
}

inline
void WinEdit::SetHScroll(bool Value)
{
	SetStyle(WS_HSCROLL, Value);
}

inline
bool WinEdit::GetVScroll()
{
	return GetStyle(WS_VSCROLL);
}

inline
void WinEdit::SetVScroll(bool Value)
{
	SetStyle(WS_VSCROLL, Value);
}

inline
bool WinEdit::GetReadonly()
{
	return GetStyle(ES_READONLY);
}

inline
void WinEdit::SetReadonly(bool Value)
{
	SendMessageW(handle, EM_SETREADONLY, (Value ? TRUE : FALSE), 0);
}

inline
bool WinEdit::GetWantReturn()
{
	return GetStyle(ES_WANTRETURN);
}

inline
void WinEdit::SetWantReturn(bool Value)
{
	SetStyle(ES_WANTRETURN, Value);
}

inline
bool WinEdit::GetBorder()
{
	return GetStyle(WS_BORDER);
}

inline
void WinEdit::SetBorder(bool Value)
{
	SetStyle(WS_BORDER, Value);
}

inline
int WinEdit::GetTextLimit()
{
	return SendMessageW(handle, EM_GETLIMITTEXT, 0, 0);
}

inline
void WinEdit::SetTextLimit(int Value)
{
	SendMessageW(handle, EM_SETLIMITTEXT, Value, 0);
}

inline
bool WinEdit::GetModified()
{
	return SendMessageW(handle, EM_GETMODIFY, 0, 0) != 0;
}

inline
void WinEdit::SetModified(bool Value)
{
	SendMessageW(handle, EM_SETMODIFY, Value ? TRUE : FALSE, 0);
}

inline
std::wstring WinEdit::GetSel()
{
	return std::wstring(GetText(), GetSelStart(), GetSelLength());
}

inline
void WinEdit::SetSel(const std::wstring& Sel)
{
	SendMessageW(handle, EM_REPLACESEL, TRUE, (LPARAM)Sel.c_str());
}

inline
int WinEdit::GetSelStart()
{
	return LOWORD(SendMessageW(handle, EM_GETSEL, 0, 0));
}

inline
void WinEdit::SetSelStart(int Value)
{
	Select(Value, 0);
}

inline
int WinEdit::GetSelLength()
{
	LRESULT Result = SendMessageW(handle, EM_GETSEL, 0, 0);
	return HIWORD(Result) - LOWORD(Result);
}

inline
void WinEdit::SetSelLength(int Value)
{
	Select(GetSelStart(), Value);
}

inline
void WinEdit::Select(int Index, int Count)
{
	SendMessageW(handle, EM_SETSEL, Index, Index + Count);
}

inline
void WinEdit::SelectAll()
{
	SendMessageW(handle, EM_SETSEL, 0, -1);
}

inline
void WinEdit::ClearSel()
{
	SendMessageW(handle, WM_CLEAR, 0, 0);
}

inline
void WinEdit::Copy()
{
	SendMessageW(handle, WM_COPY, 0, 0);
}

inline
void WinEdit::Cut()
{
	SendMessageW(handle, WM_CUT, 0, 0);
}

inline
void WinEdit::Paste()
{
	SendMessageW(handle, WM_PASTE, 0, 0);
}

inline
bool WinEdit::GetCanUndo()
{
	return SendMessageW(handle, EM_CANUNDO, 0, 0) != 0;
}

inline
int WinEdit::GetCharFromPos(int X, int Y)
{
	return LOWORD(SendMessageW(handle, EM_CHARFROMPOS, 0, MAKEWORD(X, Y)));
}

inline
void WinEdit::GetPosFromChar(int Index, int& X, int& Y)
{
	LRESULT Result = SendMessageW(handle, EM_POSFROMCHAR, Index, 0);
	X = LOWORD(Result);
	Y = HIWORD(Result);
}

inline
int WinEdit::GetFirstVisible()
{
	return SendMessageW(handle, EM_GETFIRSTVISIBLELINE, 0, 0);
}

inline
int WinEdit::GetLineCount()
{
	return SendMessageW(handle, EM_GETLINECOUNT, 0, 0);
}

inline
int WinEdit::GetCurrentLine()
{
	return GetLineIndex(-1);
}

inline
int WinEdit::GetLineIndex(int CharIndex)
{
	return SendMessageW(handle, EM_LINEFROMCHAR, CharIndex, 0);
}

inline
int WinEdit::GetCharIndex(int LineIndex)
{
	return SendMessageW(handle, EM_LINEINDEX, LineIndex, 0);
}

inline
void WinEdit::ScrollToCaret()
{
	SendMessageW(handle, EM_SCROLLCARET, 0, 0);
}

inline
void WinEdit::ClearUndoBuffer()
{
	SendMessageW(handle, EM_EMPTYUNDOBUFFER, 0, 0);
}

inline
void WinEdit::Undo()
{
	SendMessageW(handle, WM_UNDO, 0, 0);
}

inline
int WinListBox::GetFocusedIndex()
{
	return SendMessageW(handle, LB_GETCARETINDEX, 0, 0);
}

inline
void WinListBox::SetFocusedIndex(int Value)
{
	SendMessageW(handle, LB_SETCARETINDEX, Value, FALSE);
}

inline
void WinListBox::SetSelectedIndex(int Value)
{
	if (!IsMultiSelection())
	{
		SendMessageW(handle, LB_SETCURSEL, Value, 0);
	}
}

inline
int WinListBox::GetItemHeight(int Index)
{
	if (!GetStyle(LBS_OWNERDRAWVARIABLE))Index = 0;
	return SendMessageW(handle, LB_GETITEMHEIGHT, Index, 0);
}

inline
void WinListBox::SetItemHeight(int Index, int Value)
{
	if (!GetStyle(LBS_OWNERDRAWVARIABLE))Index = 0;
	SendMessageW(handle, LB_SETITEMHEIGHT, Index, Value);
}

inline
int WinListBox::GetFirstVisibleIndex()
{
	return SendMessageW(handle, LB_GETTOPINDEX, 0, 0);
}

inline
void WinListBox::SetFirstVisibleIndex(int Value)
{
	SendMessageW(handle, LB_SETTOPINDEX, Value, 0);
}

inline
bool WinListBox::GetSelected(int Index)
{
	return SendMessageW(handle, LB_GETSEL, 0, 0)>0;
}

inline
void WinListBox::SetSelected(int Index, bool Value)
{
	SendMessageW(handle, LB_SETSEL, (Value ? TRUE : FALSE), Index);
}

inline
void WinListBox::SelectAll(bool Selected)
{
	SetSelected(-1, Selected);
}

inline
void WinListBox::AddString(const std::wstring& String)
{
	SendMessageW(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(String.c_str()));
}

inline
void WinListBox::InsertString(int Index, const std::wstring& String)
{
	if (Index == GetCount())Index = -1;
	SendMessageW(handle, LB_INSERTSTRING, Index, reinterpret_cast<LPARAM>(String.c_str()));
}

inline
void WinListBox::DeleteString(int Index)
{
	SendMessageW(handle, LB_DELETESTRING, Index, 0);
}

inline
int WinListBox::GetCount()
{
	return SendMessageW(handle, LB_GETCOUNT, 0, 0);
}

inline
void WinListBox::SetString(int Index, const std::wstring& String)
{
	DeleteString(Index);
	InsertString(Index, String);
}

inline
void WinListBox::Clear()
{
	SendMessageW(handle, LB_RESETCONTENT, 0, 0);
}

inline
int WinListBox::FindStringPrefix(const std::wstring& String)
{
	int Result = SendMessageW(handle, LB_FINDSTRING, -1, reinterpret_cast<LPARAM>(String.c_str()));
	return Result == LB_ERR ? -1 : Result;
}

inline
int WinListBox::FindString(const std::wstring& String)
{
	int Result = SendMessageW(handle, LB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(String.c_str()));
	return Result == LB_ERR ? -1 : Result;
}

inline
void WinListBox::SelectPrefix(const std::wstring& String)
{
	SendMessageW(handle, LB_SELECTSTRING, -1, reinterpret_cast<LPARAM>(String.c_str()));
}

inline
bool WinComboBox::GetDisabledScroll()
{
	return GetStyle(CBS_DISABLENOSCROLL);
}

inline
void WinComboBox::SetDisabledScroll(bool Value)
{
	SetStyle(CBS_DISABLENOSCROLL, Value);
}

inline
int WinComboBox::GetSelStart()
{
	DWORD Start, End;
	SendMessageW(handle, CB_GETEDITSEL, reinterpret_cast<WPARAM>(&Start), reinterpret_cast<LPARAM>(&End));
	return Start;
}

inline
void WinComboBox::SetSelStart(int Value)
{
	Select(Value, 0);
}

inline
int WinComboBox::GetSelLength()
{
	DWORD Start, End;
	SendMessageW(handle, CB_GETEDITSEL, reinterpret_cast<WPARAM>(&Start), reinterpret_cast<LPARAM>(&End));
	return End - Start;
}

inline
void WinComboBox::SetSelLength(int Value)
{
	Select(GetSelStart(), Value);
}

inline
void WinComboBox::Select(int Index, int Length)
{
	SendMessageW(handle, CB_SETEDITSEL, 0, MAKEWORD(Index, Index + Length));
}

inline
int WinComboBox::GetMinVisible()
{
	return SendMessageW(handle, CB_GETMINVISIBLE, 0, 0);
}

inline
void WinComboBox::SetMinVisible(int Value)
{
	SendMessageW(handle, CB_SETMINVISIBLE, Value, 0);
}

inline
void WinComboBox::SelectAll()
{
	Select(0, -1);
}

inline
void WinComboBox::ClearSel()
{
	Select(-1, 0);
}

inline
int WinComboBox::GetSelectedIndex()
{
	return SendMessageW(handle, CB_GETCURSEL, 0, 0);
}

inline
void WinComboBox::SetSelectedIndex(int Index)
{
	SendMessageW(handle, CB_SETCURSEL, Index, 0);
	OnSelChanged(this);
}

inline
int WinComboBox::GetItemHeight()
{
	return SendMessageW(handle, CB_GETITEMHEIGHT, 0, 0);
}

inline
void WinComboBox::SetItemHeight(int Value)
{
	SendMessageW(handle, CB_SETITEMHEIGHT, 0, Value);
}

inline
int WinComboBox::GetFirstVisibleIndex()
{
	return SendMessageW(handle, CB_GETTOPINDEX, 0, 0);
}

inline
void WinComboBox::SetFirstVisibleIndex(int Index)
{
	SendMessageW(handle, CB_SETTOPINDEX, Index, 0);
}

inline
bool WinComboBox::IsOpened()
{
	return SendMessageW(handle, CB_GETDROPPEDSTATE, 0, 0) == TRUE;
}

inline
void WinComboBox::AddString(const std::wstring& String)
{
	SendMessageW(handle, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(String.c_str()));
}

inline
void WinComboBox::InsertString(int Index, const std::wstring& String)
{
	SendMessageW(handle, CB_INSERTSTRING, Index, reinterpret_cast<LPARAM>(String.c_str()));
}

inline
void WinComboBox::DeleteString(int Index)
{
	SendMessageW(handle, CB_DELETESTRING, Index, 0);
}

inline
int WinComboBox::GetCount()
{
	return SendMessageW(handle, CB_GETCOUNT, 0, 0);
}

inline
int WinComboBox::FindStringPrefix(const std::wstring& String)
{
	LRESULT Result = SendMessageW(handle, CB_FINDSTRING, -1, reinterpret_cast<LPARAM>(String.c_str()));
	return Result == CB_ERR ? -1 : Result;
}

inline
int WinComboBox::FindString(const std::wstring& String)
{
	LRESULT Result = SendMessageW(handle, CB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(String.c_str()));
	return Result == CB_ERR ? -1 : Result;
}

inline
void WinComboBox::SelectPrefix(const std::wstring& String)
{
	SendMessageW(handle, CB_SELECTSTRING, -1, reinterpret_cast<LPARAM>(String.c_str()));
}

inline
void WinComboBox::OpenList()
{
	SendMessageW(handle, CB_SHOWDROPDOWN, TRUE, 0);
}

inline
void WinComboBox::CloseList()
{
	SendMessageW(handle, CB_SHOWDROPDOWN, FALSE, 0);
}

