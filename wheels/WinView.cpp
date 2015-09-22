#include "WinView.h"

namespace pl
{
namespace windows
{

const int LISTVIEW_TEXT_BUFFER_SIZE = 1024;
IMPLEMENT_VOID_EVENT(ListViewItemEvent, (Sender, ItemIndex, SubItemIndex), (Object* Sender, int ItemIndex, int SubItemIndex));
IMPLEMENT_VOID_EVENT(ListViewEditEvent, (Sender, ItemIndex, Accept, Text), (Object* Sender, int ItemIndex, bool& Accept, const std::wstring& Text));
IMPLEMENT_VOID_EVENT(ListViewSortEvent, (Sender, Index1, Index2, Param, Result), (Object* Sender, int Index1, int Index2, void* Param, ListViewSortResult& Result));

IMPLEMENT_VOID_EVENT(TreeViewItemEvent, (Sender, Item), (Object* Sender, WinTreeViewItem Item));
IMPLEMENT_VOID_EVENT(TreeViewVerifyEvent, (Sender, Item, Accept), (Object* Sender, WinTreeViewItem Item, bool& Accept));
IMPLEMENT_VOID_EVENT(TreeViewEditEvent, (Sender, Item, Accept, Text), (Object* Sender, WinTreeViewItem Item, bool& Accept, const std::wstring& Text));

void FillListViewItem(LVITEMW* Item, int Index, int SubItem, UINT Mask)
{
	memset(Item, 0, sizeof(*Item));
	Item->iItem = Index;
	Item->iSubItem = SubItem;
	Item->mask = Mask;
}

WinListViewItem::WinListViewItem(HWND Handle, int Index)
	: _Handle(Handle)
	, _Index(Index)
{
}

WinListViewItem::WinListViewItem()
	: WinListViewItem(0, 0)
{
}

WinListViewItem::WinListViewItem(const WinListViewItem& Item)
	: WinListViewItem(Item._Handle, Item._Index)
{
}

WinListViewItem& WinListViewItem::operator=(const WinListViewItem& Item)
{
	_Handle = Item._Handle;
	_Index = Item._Index;
	return *this;
}

int WinListViewItem::GetImageIndex()
{
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_IMAGE);
	ListView_GetItem(_Handle, &Item);
	return Item.iImage;
}

void WinListViewItem::SetImageIndex(int Value)
{
	LVITEM Item;
	FillListViewItem(&Item, _Index, 0, LVIF_IMAGE);
	Item.iImage = Value;
	ListView_SetItem(_Handle, &Item);
}

std::wstring WinListViewItem::GetSubItem(int Index)
{
	wchar_t Buffer[LISTVIEW_TEXT_BUFFER_SIZE] = {0};
	LVITEMW Item;
	FillListViewItem(&Item, _Index, Index + 1, LVIF_TEXT);
	Item.pszText = Buffer;
	Item.cchTextMax = sizeof(Buffer) / sizeof(*Buffer);
	ListView_GetItem(_Handle, &Item);
	return Buffer;
}

void WinListViewItem::SetSubItem(int Index, std::wstring& Value)
{
	LVITEMW Item;
	FillListViewItem(&Item, _Index, Index + 1, LVIF_TEXT);
	Item.pszText = &Value[0];
	ListView_SetItem(_Handle, &Item);
}

void WinListViewItem::GetTiledSubItem(std::vector<int>& Columns)
{
	UINT Buffer[20] = {0};
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_COLUMNS);
	Item.puColumns = Buffer;
	ListView_GetItem(_Handle, &Item);
	Columns.clear();
	for (UINT i = 0; i < Item.cColumns; i++)
	{
		Columns.push_back(Buffer[i] - 1);
	}
}

void WinListViewItem::SetTiledSubItem(std::vector<int>& Columns)
{
	UINT Buffer[20] = {0};
	size_t Count = 0;
	for (; Count < Columns.size() && Count < 20; Count++)
	{
		Buffer[Count] = Columns[Count] + 1;
	}
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_COLUMNS);
	Item.cColumns = Count;
	Item.puColumns = Buffer;
	ListView_SetItem(_Handle, &Item);
}

int WinListViewItem::GetIndent()
{
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_INDENT);
	ListView_GetItem(_Handle, &Item);
	return Item.iIndent;
}

void WinListViewItem::SetIndent(int Value)
{
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_INDENT);
	Item.iIndent = Value;
	ListView_SetItem(_Handle, &Item);
}

void* WinListViewItem::GetCustomData()
{
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_PARAM);
	ListView_GetItem(_Handle, &Item);
	return reinterpret_cast<void*>(Item.lParam);
}

void WinListViewItem::SetCustomData(void* Data)
{
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_PARAM);
	Item.lParam = reinterpret_cast<LPARAM>(Data);
	ListView_SetItem(_Handle, &Item);
}

void WinListViewItem::EnterGroup(int GroupID)
{
	LVITEMW Item;
	FillListViewItem(&Item, _Index, 0, LVIF_GROUPID);
	Item.iGroupId = GroupID;
	ListView_SetItem(_Handle, &Item);
}

int WinListViewItem::GetOwnerGroupID()
{
	LVITEM Item;
	FillListViewItem(&Item, _Index, 0, LVIF_GROUPID);
	ListView_GetItem(_Handle, &Item);
	return Item.iGroupId;
}

}
}
