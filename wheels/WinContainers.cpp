#include "WinContainers.h"

namespace pl
{
namespace windows
{

static const int TABPAGE_TEXT_BUFFER_SIZE = 1024;

WinGroup::WinGroup(WinContainer* Parent)
	: WinContainer()
{
	_CreateWindow(WS_EX_CONTROLPARENT,
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


}
}