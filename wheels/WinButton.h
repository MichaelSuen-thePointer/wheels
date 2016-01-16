#pragma once
#ifndef WINBUTTON_H
#define WINBUTTON_H

#include "WinClass.h"
#include "commctrl.h"

namespace pl
{
namespace windows
{

class WinCustomButton: public WinControl
{
public:
	NotifyEvent OnClick;

	WinCustomButton(WinContainer* Parent, DWORD Style);

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	Align GetTextVAlign();
	void SetTextVAlign(Align Value);

	Align GetTextHAlign();
	void SetTextHAlign(Align Value);
};

class WinCustomChecked: public WinCustomButton
{
public:
	WinCustomChecked(WinContainer* Parent, DWORD Style);

	bool GetChecked();
	virtual void SetChecked(bool Value);

	bool GetLeftText();
	void SetLeftText(bool Value);

	bool GetPushLike();
	void SetPushLike(bool Value);
};

class WinButton: public WinCustomButton
{
public:
	WinButton(WinContainer* Parent);
};

class WinCommandLink: public WinCustomButton
{
public:
	WinCommandLink(WinContainer* Parent);

	std::wstring GetNote();
	void SetNote(const std::wstring& Note);
};

class WinCheck: public WinCustomChecked
{
public:
	WinCheck(WinContainer* Parent);
};

class WinRadio: public WinCustomChecked
{
protected:
	int groupNumber;
public:
	WinRadio(WinContainer* Parent);

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	void SetChecked(bool Value) override;

	int GetGroupNumber();
	void SetGroupNumber(int Value);
};

class WinSplitButton: public WinCustomButton
{
public:
	DropDownEvent OnDropDown;

	WinSplitButton(WinContainer* Parent);

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;
};

#include "WinButton.inl"

} //namespace windows
} //namespace pl

#endif // !WINBUTTON_H
