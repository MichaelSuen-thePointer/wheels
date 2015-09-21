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

WinListViewItem::operator bool()
{
	return _Handle != 0;
}

int WinListViewItem::GetIndex()
{
	return _Index;
}






}
}
