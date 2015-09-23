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
void FillListViewColumn(LVCOLUMN* Column, UINT Mask)
{
	memset(Column, 0, sizeof(*Column));
	Column->mask = Mask;
}
WinListViewColumn::WinListViewColumn(HWND Handle, int Index)
	: _Handle(Handle)
	, _Index(Index)
{
}
WinListViewColumn::WinListViewColumn()
	: WinListViewColumn(0, 0)
{
}
WinListViewColumn::WinListViewColumn(const WinListViewColumn& Item)
	: WinListViewColumn(Item._Handle, Item._Index)
{
}
WinListViewColumn& WinListViewColumn::operator=(const WinListViewColumn& Item)
{
	_Handle = Item._Handle;
	_Index = Item._Index;
	return *this;
}
std::wstring WinListViewColumn::GetText()
{
	wchar_t Buffer[LISTVIEW_TEXT_BUFFER_SIZE] = {0};
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_TEXT);
	Column.pszText = Buffer;
	Column.cchTextMax = sizeof(Buffer) / sizeof(*Buffer);
	ListView_GetColumn(_Handle, _Index, &Column);
	return Buffer;
}
void WinListViewColumn::SetText(std::wstring& Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_TEXT);
	Column.pszText = &Value[0];
	ListView_SetColumn(_Handle, _Index, &Column);
}
int WinListViewColumn::GetSubItemIndex()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_SUBITEM);
	ListView_GetColumn(_Handle, _Index, &Column);
	return Column.iSubItem - 1;
}
void WinListViewColumn::SetSubItemIndex(int Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_SUBITEM);
	Column.iSubItem = Value + 1;
	ListView_SetColumn(_Handle, _Index, &Column);
}
int WinListViewColumn::GetWidth()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_WIDTH);
	ListView_GetColumn(_Handle, _Index, &Column);
	return Column.cx;
}
void WinListViewColumn::SetWidth(int Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_WIDTH | LVCF_DEFAULTWIDTH);
	Column.cx = Value;
	Column.cxDefault = Value;
	ListView_SetColumn(_Handle, _Index, &Column);
}
int WinListViewColumn::GetOrder()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_ORDER);
	ListView_GetColumn(_Handle, _Index, &Column);
	return Column.iOrder;
}
void WinListViewColumn::SetOrder(int Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_ORDER);
	Column.iOrder = Value;
	ListView_SetColumn(_Handle, _Index, &Column);
}
int WinListViewColumn::GetImageIndex()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_IMAGE);
	ListView_GetColumn(_Handle, _Index, &Column);
	return Column.iImage;
}
void WinListViewColumn::SetImageIndex(int Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_IMAGE | LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	if (Value >= 0)
	{
		Column.fmt |= LVCFMT_COL_HAS_IMAGES;
	}
	else
	{
		Column.fmt &= ~LVCFMT_COL_HAS_IMAGES;
	}
	Column.iImage = Value;
	ListView_SetColumn(_Handle, _Index, &Column);
}
WinHAlign WinListViewColumn::GetTextAlign()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	if (Column.fmt & LVCFMT_RIGHT)
	{
		return WinHAlign::Right;
	}
	else if (Column.fmt & LVCFMT_CENTER)
	{
		return WinHAlign::Center;
	}
	else
	{
		return WinHAlign::Left;
	}
}
void WinListViewColumn::SetTextAlign(WinHAlign Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	Column.fmt &= ~LVCFMT_RIGHT;
	Column.fmt &= ~LVCFMT_CENTER;
	Column.fmt &= ~LVCFMT_LEFT;
	switch (Value)
	{
	case WinHAlign::Right:
		Column.fmt |= LVCFMT_RIGHT;
		break;
	case WinHAlign::Center:
		Column.fmt |= LVCFMT_CENTER;
		break;
	case WinHAlign::Left:
		Column.fmt |= LVCFMT_LEFT;
		break;
	}
	ListView_SetColumn(_Handle, _Index, &Column);
}
bool WinListViewColumn::GetImageOnRight()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	return (Column.fmt & LVCFMT_BITMAP_ON_RIGHT) != 0;
}
void WinListViewColumn::SetImageOnRight(bool Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	if (Value)
	{
		Column.fmt |= LVCFMT_BITMAP_ON_RIGHT;
	}
	else
	{
		Column.fmt &= ~LVCFMT_BITMAP_ON_RIGHT;
	}
	ListView_SetColumn(_Handle, _Index, &Column);
}
bool WinListViewColumn::GetFixedWidth()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	return (Column.fmt & LVCFMT_FIXED_WIDTH) != 0;
}
void WinListViewColumn::SetFixedWidth(bool Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	if (Value)
	{
		Column.fmt |= LVCFMT_FIXED_WIDTH;
	}
	else
	{
		Column.fmt &= ~LVCFMT_FIXED_WIDTH;
	}
	ListView_SetColumn(_Handle, _Index, &Column);
}
bool WinListViewColumn::GetShowSplitButton()
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	return (Column.fmt & LVCFMT_SPLITBUTTON) != 0;
}
void WinListViewColumn::SetShowSplitButton(bool Value)
{
	LVCOLUMNW Column;
	FillListViewColumn(&Column, LVCF_FMT);
	ListView_GetColumn(_Handle, _Index, &Column);
	if (Value)
	{
		Column.fmt |= LVCFMT_SPLITBUTTON;
	}
	else
	{
		Column.fmt &= ~LVCFMT_SPLITBUTTON;
	}
	ListView_SetColumn(_Handle, _Index, &Column);
}

void FillListViewGroup(LVGROUP* Group, UINT Mask)
{
	memset(Group, 0, sizeof(*Group));
	Group->cbSize = sizeof(*Group);
	Group->mask = Mask;
}

std::wstring WinListViewGroup::GetListViewGroupText(UINT Mask, LPWSTR LVGROUP::* Item, UINT LVGROUP::* Count)
{
	wchar_t Buffer[LISTVIEW_TEXT_BUFFER_SIZE];
	LVGROUP Group;
	FillListViewGroup(&Group, Mask);
	Group.*Item = Buffer;
	Group.*Count = LISTVIEW_TEXT_BUFFER_SIZE;
	ListView_GetGroupInfo(_Handle, _ID, &Group);
	return Buffer;
}

std::wstring WinListViewGroup::GetListViewGroupText(UINT Mask, LPWSTR LVGROUP::* Item, int LVGROUP::* Count)
{
	wchar_t Buffer[LISTVIEW_TEXT_BUFFER_SIZE];
	LVGROUP Group;
	FillListViewGroup(&Group, Mask);
	Group.*Item = Buffer;
	Group.*Count = LISTVIEW_TEXT_BUFFER_SIZE;
	ListView_GetGroupInfo(_Handle, _ID, &Group);
	return Buffer;
}

void WinListViewGroup::SetListViewGroupText(UINT Mask, LPWSTR LVGROUP::* Item, std::wstring& Text)
{
	LVGROUP Group;
	FillListViewGroup(&Group, Mask);
	Group.*Item = Text.length() ? &Text[0] : NULL;
	ListView_SetGroupInfo(_Handle, _ID, &Group);
}
WinListViewGroup::WinListViewGroup(HWND Handle, int ID)
	: _Handle(Handle)
	, _ID(ID)
{
}
WinListViewGroup::WinListViewGroup()
	: WinListViewGroup(0, 0)
{
}
WinListViewGroup::WinListViewGroup(const WinListViewGroup& Item)
	: WinListViewGroup(Item._Handle, Item._ID)
{
}
WinListViewGroup& WinListViewGroup::operator=(const WinListViewGroup& Item)
{
	_Handle = Item._Handle;
	_ID = Item._ID;
	return *this;
}


}
}
