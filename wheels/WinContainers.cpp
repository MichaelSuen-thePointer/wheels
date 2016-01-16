#include "WinContainers.h"

namespace pl
{
namespace windows
{

static const int TABPAGE_TEXT_BUFFER_SIZE = 1024;

WinGroup::WinGroup(WinContainer* Parent)
	: WinContainer()
{
	InternalCreateWindow(WS_EX_CONTROLPARENT,
				  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
				  WC_BUTTON,
				  Parent);
}

WinGroup::~WinGroup()
{}

inline
void FillTabPageItem(TCITEMW* Item, UINT Mask)
{
	memset(Item, 0, sizeof(*Item));
	Item->mask = Mask;
}

WinTabPage::WinTabPage(HWND Handle, int Index, WinContainer* Container)
	: _Handle(Handle)
	, _Index(Index)
	, _Container(Container)
{}

WinTabPage::WinTabPage()
	: WinTabPage(0, 0, nullptr)
{}

WinTabPage::WinTabPage(const WinTabPage& Page)
	: WinTabPage(Page._Handle, Page._Index, Page._Container)
{}

WinTabPage::WinTabPage(WinTabPage&& Page)
	: WinTabPage(Page._Handle, Page._Index, Page._Container)
{
	Page._Handle = 0;
	Page._Index = 0;
	Page._Container = nullptr;
}

WinTabPage& WinTabPage::operator=(const WinTabPage& Page)
{
	_Handle = Page._Handle;
	_Index = Page._Index;
	_Container = Page._Container;
	return *this;
}

WinTabPage& WinTabPage::operator=(WinTabPage&& Page)
{
	_Handle = Page._Handle;
	_Index = Page._Index;
	_Container = Page._Container;

	Page._Handle = 0;
	Page._Index = 0;
	Page._Container = nullptr;
	return *this;
}

WinTabPage::operator bool()
{
	return _Handle != 0;
}

WinTabPage::operator WinContainer*()
{
	return _Container;
}

std::wstring WinTabPage::GetText()
{
	wchar_t Buffer[TABPAGE_TEXT_BUFFER_SIZE] = {0};
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_TEXT);
	Item.pszText = Buffer;
	Item.cchTextMax = TABPAGE_TEXT_BUFFER_SIZE;
	TabCtrl_GetItem(_Handle, _Index, &Item);
	return Buffer;
}

void WinTabPage::SetText(std::wstring& Value)
{
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_TEXT);
	Item.pszText = &Value[0];
	TabCtrl_SetItem(_Handle, _Index, &Item);
}

int WinTabPage::GetImageIndex()
{
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_IMAGE);
	TabCtrl_GetItem(_Handle, _Index, &Item);
	return Item.iImage;
}

void WinTabPage::SetImageIndex(int Value)
{
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_IMAGE);
	Item.iImage = Value;
	TabCtrl_SetItem(_Handle, _Index, &Item);
}

void* WinTabPage::GetCustomData()
{
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_PARAM);
	TabCtrl_GetItem(_Handle, _Index, &Item);
	return reinterpret_cast<void*>(Item.lParam);
}

void WinTabPage::SetCustomData(void* Value)
{
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_PARAM);
	Item.lParam = reinterpret_cast<LPARAM>(Value);
	TabCtrl_SetItem(_Handle, _Index, &Item);
}

bool WinTabPage::GetHighlighted()
{
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_STATE);
	Item.dwStateMask = TCIS_HIGHLIGHTED;
	TabCtrl_GetItem(_Handle, _Index, &Item);
	return Item.dwState != 0;
}

void WinTabPage::SetHighlighted(bool Value)
{
	TabCtrl_HighlightItem(_Handle, _Index, (Value ? TRUE : FALSE));
}

DWORD WinTab::InternalGetExStyle()
{
	return TabCtrl_GetExtendedStyle(handle);
}

void WinTab::InternalSetExStyle(DWORD ExStyle)
{
	TabCtrl_SetExtendedStyle(handle, ExStyle);
}

void WinTab::GetClientArea(RECT* Rect)
{
	Rect->left = 0;
	Rect->top = 0;
	Rect->right = GetWidth();
	Rect->bottom = GetHeight();
	TabCtrl_AdjustRect(handle, FALSE, Rect);
}

void WinTab::ArrangeTabContainers()
{
	RECT Rect;
	GetClientArea(&Rect);
	for (auto& Elem : _TabContainers)
	{
		Elem->Move(Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top);
	}
}

void WinTab::ResetTopTabContainer()
{
	int Index = TabCtrl_GetCurSel(handle);
	if (Index != -1)
	{
		for (auto& Elem : _TabContainers)
		{
			Elem->SetVisible(false);
		}
		_TabContainers[Index]->SetVisible(true);
	}
}

WinTab::WinTab(WinContainer* Parent)
	: WinContainer()
	, _ImageList(nullptr)
	, _TabContainers()
{
	InternalCreateWindow(WS_EX_CONTROLPARENT,
				  WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_CHILD | TCS_FOCUSONBUTTONDOWN,
				  WC_TABCONTROL,
				  Parent);
	TabCtrl_SetUnicodeFormat(handle, TRUE);
}

WinTab::~WinTab()
{}

LRESULT WinTab::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_NOTIFY_DISPATCHED:
		switch (reinterpret_cast<NMHDR*>(lParam)->code)
		{
		case NM_CLICK:
			OnClick(this);
			break;
		case NM_DBLCLK:
			OnDblClick(this);
			break;
		case NM_RCLICK:
			OnRightClick(this);
			break;
		case NM_RDBLCLK:
			OnRightDblClick(this);
			break;
		case TCN_SELCHANGE:
			ResetTopTabContainer();
			OnSelChanged(this);
			break;
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	case WM_SIZE:
		ArrangeTabContainers();
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

WinTabPage WinTab::InsertPage(int Index, std::wstring& Text, WinContainer* Container)
{
	TCITEMW Item;
	FillTabPageItem(&Item, TCIF_TEXT);
	Item.pszText = &Text[0];
	Index = TabCtrl_InsertItem(handle, Index, &Item);
	if (Index == -1)
	{
		return WinTabPage();
	}
	else
	{
		RECT Rect;
		GetClientArea(&Rect);
		if (Container)
		{
			Container->SetParent(this);
		}
		else
		{
			Container = new WinStatic(this);
		}
		_TabContainers.insert(_TabContainers.begin() + Index, Container);
		ResetTopTabContainer();
		ArrangeTabContainers();
		return WinTabPage(handle, Index, Container);
	}
}

void WinTab::DeletePage(int Index)
{
	assert(Index >= 0 && Index < GetPageCount());
	if (TabCtrl_DeleteItem(handle, Index) == TRUE)
	{
		delete _TabContainers[Index];
		_TabContainers.erase(_TabContainers.begin() + Index);
		ArrangeTabContainers();
	}
}

WinTabPage WinTab::GetPage(int Index)
{
	assert(Index >= 0 && Index < GetPageCount());
	return WinTabPage(handle, Index, _TabContainers[Index]);
}

WinTabPage WinTab::GetSelectedPage()
{
	int Index = TabCtrl_GetCurSel(handle);
	if (Index == -1)
	{
		return WinTabPage();
	}
	return WinTabPage(handle, Index, _TabContainers[Index]);
}

void WinTab::SetSelectedPage(WinTabPage Page)
{
	if (Page)
	{
		TabCtrl_SetCurSel(handle, Page._Index);
		ResetTopTabContainer();
		OnSelChanged(this);
	}
}



}
}