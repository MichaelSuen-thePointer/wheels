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
	return static_cast<wchar_t>(SendMessageW(_Handle, EM_GETPASSWORDCHAR, 0, 0));
}

inline
void WinEdit::SetPasswordChar(wchar_t Char)
{
	SendMessageW(_Handle, EM_SETPASSWORDCHAR, Char, 0);
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
	SendMessageW(_Handle, EM_SETREADONLY, (Value ? TRUE : FALSE), 0);
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
	return SendMessageW(_Handle, EM_GETLIMITTEXT, 0, 0);
}

inline
void WinEdit::SetTextLimit(int Value)
{
	SendMessageW(_Handle, EM_SETLIMITTEXT, Value, 0);
}

inline
bool WinEdit::GetModified()
{
	return SendMessageW(_Handle, EM_GETMODIFY, 0, 0) != 0;
}

inline
void WinEdit::SetModified(bool Value)
{
	SendMessageW(_Handle, EM_SETMODIFY, Value ? TRUE : FALSE, 0);
}

inline
std::wstring WinEdit::GetSel()
{
	return std::wstring(GetText(), GetSelStart(), GetSelLength());
}

inline
void WinEdit::SetSel(const std::wstring& Sel)
{
	SendMessageW(_Handle, EM_REPLACESEL, TRUE, (LPARAM)Sel.c_str());
}

inline
int WinEdit::GetSelStart()
{
	return LOWORD(SendMessageW(_Handle, EM_GETSEL, 0, 0));
}

inline
void WinEdit::SetSelStart(int Value)
{
	Select(Value, 0);
}

inline
int WinEdit::GetSelLength()
{
	LRESULT Result = SendMessageW(_Handle, EM_GETSEL, 0, 0);
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
	SendMessageW(_Handle, EM_SETSEL, Index, Index + Count);
}

inline
void WinEdit::SelectAll()
{
	SendMessageW(_Handle, EM_SETSEL, 0, -1);
}

inline
void WinEdit::ClearSel()
{
	SendMessageW(_Handle, WM_CLEAR, 0, 0);
}

inline
void WinEdit::Copy()
{
	SendMessageW(_Handle, WM_COPY, 0, 0);
}

inline
void WinEdit::Cut()
{
	SendMessageW(_Handle, WM_CUT, 0, 0);
}

inline
void WinEdit::Paste()
{
	SendMessageW(_Handle, WM_PASTE, 0, 0);
}

inline
bool WinEdit::GetCanUndo()
{
	return SendMessageW(_Handle, EM_CANUNDO, 0, 0) != 0;
}

inline
int WinEdit::GetCharFromPos(int X, int Y)
{
	return LOWORD(SendMessageW(_Handle, EM_CHARFROMPOS, 0, MAKEWORD(X, Y)));
}

inline
void WinEdit::GetPosFromChar(int Index, int& X, int& Y)
{
	LRESULT Result = SendMessage(_Handle, EM_POSFROMCHAR, Index, 0);
	X = LOWORD(Result);
	Y = HIWORD(Result);
}

inline
int WinEdit::GetFirstVisible()
{
	return SendMessage(_Handle, EM_GETFIRSTVISIBLELINE, 0, 0);
}

inline
int WinEdit::GetLineCount()
{
	return SendMessageW(_Handle, EM_GETLINECOUNT, 0, 0);
}

inline
int WinEdit::GetCurrentLine()
{
	return GetLineIndex(-1);
}

inline
int WinEdit::GetLineIndex(int CharIndex)
{
	return SendMessageW(_Handle, EM_LINEFROMCHAR, CharIndex, 0);
}

inline
int WinEdit::GetCharIndex(int LineIndex)
{
	return SendMessageW(_Handle, EM_LINEINDEX, LineIndex, 0);
}

inline
void WinEdit::ScrollToCaret()
{
	SendMessageW(_Handle, EM_SCROLLCARET, 0, 0);
}

inline
void WinEdit::ClearUndoBuffer()
{
	SendMessageW(_Handle, EM_EMPTYUNDOBUFFER, 0, 0);
}

inline
void WinEdit::Undo()
{
	SendMessageW(_Handle, WM_UNDO, 0, 0);
}

