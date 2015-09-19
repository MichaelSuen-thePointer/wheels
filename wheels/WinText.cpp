#include "WinText.h"

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
{}

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
	SetStyle(ES_NUMBER,    Display == EditCharDisplay::DigitOnly);
	SetStyle(ES_PASSWORD,  Display == EditCharDisplay::Password);
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



}
}