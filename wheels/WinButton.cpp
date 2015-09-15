#include "WinButton.h"

namespace pl
{
namespace windows
{

WinCustomButton::WinCustomButton(WinContainer* Parent, DWORD Style)
	: WinControl()
{
	_CreateWindow(0,
				  Style | BS_NOTIFY | BS_BITMAP | WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
				  WC_BUTTON,
				  Parent);
}

LRESULT WinCustomButton::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_COMMAND_DISPATCHED:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			OnClick(this);
			break;
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

Align WinCustomButton::GetTextVAlign()
{
	if (GetStyle(BS_TOP)) return Align::LeftTop;
	else if (GetStyle(BS_BOTTOM)) return Align::RightBottom;
	else return Align::Center;
}

void WinCustomButton::SetTextVAlign(Align Value)
{
	switch (Value)
	{
	case Align::LeftTop:
		SetStyle(BS_TOP, true);
		SetStyle(BS_VCENTER, false);
		SetStyle(BS_BOTTOM, false);
		break;
	case Align::Center:
		SetStyle(BS_TOP, false);
		SetStyle(BS_VCENTER, true);
		SetStyle(BS_BOTTOM, false);
		break;
	case Align::RightBottom:
		SetStyle(BS_TOP, false);
		SetStyle(BS_VCENTER, false);
		SetStyle(BS_BOTTOM, true);
		break;
	}
}

Align WinCustomButton::GetTextHAlign()
{
	if (GetStyle(BS_LEFT))			return Align::LeftTop;
	else if (GetStyle(BS_RIGHT))	return Align::RightBottom;
	else							return Align::Center;
}

void WinCustomButton::SetTextHAlign(Align Value)
{
	switch (Value)
	{
	case Align::LeftTop:
		SetStyle(BS_LEFT, true);
		SetStyle(BS_CENTER, false);
		SetStyle(BS_RIGHT, false);
		break;
	case Align::Center:
		SetStyle(BS_LEFT, false);
		SetStyle(BS_CENTER, true);
		SetStyle(BS_RIGHT, false);
		break;
	case Align::RightBottom:
		SetStyle(BS_LEFT, false);
		SetStyle(BS_CENTER, false);
		SetStyle(BS_RIGHT, true);
		break;
	}
}

WinCustomChecked::WinCustomChecked(WinContainer* Parent, DWORD Style)
	: WinCustomButton(Parent, Style)
{}

WinButton::WinButton(WinContainer* Parent)
	: WinCustomButton(Parent, BS_PUSHBUTTON | BS_CENTER | BS_VCENTER)
{}

WinCommandLink::WinCommandLink(WinContainer* Parent)
	: WinCustomButton(Parent, BS_COMMANDLINK | BS_CENTER | BS_VCENTER)
{}

std::wstring WinCommandLink::GetNote()
{
	DWORD Length = 0;
	SendMessageW(_Handle, BCM_GETNOTE, reinterpret_cast<WPARAM>(&Length), reinterpret_cast<LPARAM>(&Length));
	wchar_t* Buffer = new wchar_t[Length + 1];
	SendMessageW(_Handle, BCM_GETNOTE, reinterpret_cast<WPARAM>(&Length), reinterpret_cast<LPARAM>(Buffer));
	std::wstring Note(Buffer);
	delete[] Buffer;
	return Note;
}

void WinCommandLink::SetNote(const std::wstring& Note)
{
	SendMessageW(_Handle, BCM_SETNOTE, 0, reinterpret_cast<LPARAM>(Note.c_str()));
}

WinCheck::WinCheck(WinContainer* Parent)
	: WinCustomChecked(Parent, BS_AUTOCHECKBOX | BS_LEFT | BS_VCENTER)
{}

WinRadio::WinRadio(WinContainer* Parent)
	: WinCustomChecked(Parent, BS_RADIOBUTTON | BS_LEFT | BS_VCENTER)
	, _GroupNumber(-1)
{}

LRESULT WinRadio::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_COMMAND_DISPATCHED:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			if (!GetChecked())
			{
				SetChecked(true);
			}
			break;
		}
		Result = WinCustomButton::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinCustomButton::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

void WinRadio::SetChecked(bool Value)
{
	if (Value)
	{
		for (WinControl* Control : *_Parent)
		{
			WinRadio* Radio = dynamic_cast<WinRadio*>(Control);
			if (Radio && Radio != this && Radio->_GroupNumber == _GroupNumber)
			{
				Radio->SetChecked(false);
			}
		}
	}
	WinCustomChecked::SetChecked(Value);
}




}
}