#pragma once
#ifndef WINCONTAINERS_H
#define WINCONTAINERS_H

#include "WinClass.h"
#include "WinText.h"
namespace pl
{
namespace windows
{

class WinGroup: public WinContainer
{
public:
	WinGroup(WinContainer* Parent);
	~WinGroup();
};

class WinTabPage: public Object
{
	friend class WinTab;
protected:
	HWND _Handle;
	int _Index;
	WinContainer* _Container;

	WinTabPage(HWND Handle, int Index, WinContainer* Container);
public:
	WinTabPage();
	WinTabPage(const WinTabPage& Page);
	WinTabPage(WinTabPage&& Page);
	WinTabPage& operator=(const WinTabPage& Page);
	WinTabPage& operator=(WinTabPage&& Page);
	operator bool();
	operator WinContainer*();

	std::wstring GetText();
	void SetText(std::wstring& Value);

	int GetImageIndex();
	void SetImageIndex(int Value);

	void* GetCustomData();
	void SetCustomData(void* Value);

	bool GetHighlighted();
	void SetHighlighted(bool Value);

	int GetIndex();

	Placement* GetPlacement();
	void ApplyPlacement(placement::Base::Pointer Placement);
	int GetControlCount();
	WinControl* GetControl(int Index);
	int GetClientWidth();
	int GetClientHeight();

	WinControlList::iterator begin();
	WinControlList::iterator end();
};

class WinTab: public WinContainer
{
	using TabContainers = std::vector<WinContainer*>;
protected:
	WinImageList* _ImageList;
	TabContainers _TabContainers;

	DWORD InternalGetExStyle() override;
	void InternalSetExStyle(DWORD ExStyle) override;
	void GetClientArea(RECT* Rect) override;
	void ArrangeTabContainers();
	void ResetTopTabContainer();
public:
	WinTab(WinContainer* Parent);
	~WinTab();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	NotifyEvent OnClick;
	NotifyEvent OnDblClick;
	NotifyEvent OnRightClick;
	NotifyEvent OnRightDblClick;
	NotifyEvent OnSelChanged;

	WinImageList* GetImageList();
	void SetImageList(WinImageList* ImageList);

	bool GetButtonTab();
	void SetButtonTab(bool Value);

	bool GetFlatButton();
	void SetFlatButton(bool Value);

	bool GetFlatSeparator();
	void SetFlatSeparator(bool Value);

	bool GetMultiline();
	void SetMultiline(bool Value);

	bool GetTabAtLeftTop();
	void SetTabAtLeftTop(bool Value);

	bool GetVerticalTab();
	void SetVerticalTab(bool Value);

	WinTabPage AddPage(const std::wstring& Text, WinContainer* Container = nullptr);
	WinTabPage InsertPage(int Index, std::wstring& Text, WinContainer* Container = nullptr);
	void DeletePage(int Index);
	WinTabPage GetPage(int Index);
	int GetPageCount();
	WinTabPage GetSelectedPage();
	void SetSelectedPage(WinTabPage Page);
	int GetRowCount();
};

#include "WinContainers.inl"

}
}
#endif // !WINCONTAINERS_H
