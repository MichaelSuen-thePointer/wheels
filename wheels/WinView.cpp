#include "WinView.h"
#include <cassert>
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
int WinListViewGroup::GetImageIndex()
{
    LVGROUP Group;
    FillListViewGroup(&Group, LVGF_TITLEIMAGE);
    ListView_GetGroupInfo(_Handle, _ID, &Group);
    return Group.iTitleImage;
}
void WinListViewGroup::SetImageIndex(int Value)
{
    LVGROUP Group;
    FillListViewGroup(&Group, LVGF_TITLEIMAGE);
    Group.iTitleImage = Value;
    ListView_SetGroupInfo(_Handle, _ID, &Group);
}
int WinListViewGroup::GetExtendedImageIndex()
{
    LVGROUP Group;
    FillListViewGroup(&Group, LVGF_EXTENDEDIMAGE);
    ListView_GetGroupInfo(_Handle, _ID, &Group);
    return Group.iExtendedImage;
}
void WinListViewGroup::SetExtendedImageIndex(int Value)
{
    LVGROUP Group;
    FillListViewGroup(&Group, LVGF_EXTENDEDIMAGE);
    Group.iExtendedImage = Value;
    ListView_SetGroupInfo(_Handle, _ID, &Group);
}
WinHAlign WinListViewGroup::GetHeaderAlign()
{
    LVGROUP Group;
    FillListViewGroup(&Group, LVGF_ALIGN);
    ListView_GetGroupInfo(_Handle, _ID, &Group);
    if (Group.uAlign == LVGA_HEADER_LEFT)
    {
        return WinHAlign::Left;
    }
    else if (Group.uAlign == LVGA_HEADER_CENTER)
    {
        return WinHAlign::Center;
    }
    else
    {
        return WinHAlign::Right;
    }
}
void WinListViewGroup::SetHeaderAlign(WinHAlign Value)
{
    LVGROUP Group;
    FillListViewGroup(&Group, LVGF_ALIGN);
    switch (Value)
    {
    case WinHAlign::Left:
        Group.uAlign = LVGA_HEADER_LEFT;
        break;
    case WinHAlign::Center:
        Group.uAlign = LVGA_HEADER_CENTER;
        break;
    case WinHAlign::Right:
        Group.uAlign = LVGA_HEADER_RIGHT;
        break;
    }
    ListView_SetGroupInfo(_Handle, _ID, &Group);
}
struct ListViewSortParam
{
    void* Param;
    WinListView* ListView;
};
int CALLBACK ListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    ListViewSortResult Result = ListViewSortResult::Equal;
    ListViewSortParam* Param = reinterpret_cast<ListViewSortParam*>(lParamSort);
    Param->ListView->OnCompareItems(Param->ListView, lParam1, lParam2, Param->Param, Result);
    switch (Result)
    {
    case ListViewSortResult::Larger:
        return 1;
    case ListViewSortResult::Smaller:
        return -1;
    default:
        return 0;
    }
}
WinListView::WinListView(WinContainer* Parent)
    : WinControl()
    , _SmallImageList(nullptr)
    , _LargeImageList(nullptr)
    , _StateImageList(nullptr)
    , _GroupImageList(nullptr)
    , _ColumnCount(0)
    , _UsedGroupIDs(1)
{
    _CreateWindow(0,
                  WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_ALIGNTOP | LVS_SHAREIMAGELISTS | LVS_ICON,
                  WC_LISTVIEW,
                  Parent);
    ListView_SetUnicodeFormat(_Handle, TRUE);
    SetExStyle(LVS_EX_DOUBLEBUFFER, true);
    SetExStyle(LVS_EX_FULLROWSELECT, true);
    SetExStyle(LVS_EX_TRACKSELECT, false);
}
WinListView::~WinListView()
{
}
LRESULT WinListView::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
    LRESULT Result = 0;
    switch (Message)
    {
    case WM_NOTIFY_DISPATCHED:
        switch ((reinterpret_cast<NMHDR*>(lParam))->code)
        {
        case LVN_BEGINDRAG:
        {
            NMLISTVIEW* Param = reinterpret_cast<NMLISTVIEW*>(lParam);
            OnBeginDrag(this, Param->iItem, -1);
            break;
        }
        case LVN_BEGINRDRAG:
        {
            NMLISTVIEW* Param = reinterpret_cast<NMLISTVIEW*>(lParam);
            OnBeginRightDrag(this, Param->iItem, -1);
            break;
        }
        case LVN_BEGINLABELEDIT:
        {
            CallDefaultProc = false;
            NMLVDISPINFOW* Param = reinterpret_cast<NMLVDISPINFOW*>(lParam);
            std::wstring Text = Param->item.pszText ? Param->item.pszText : L"";
            bool Accept = true;
            OnBeginLabelEdit(this, Param->item.iItem, Accept, Text);
            return Accept ? FALSE : TRUE;
            break;
        }
        case LVN_ENDLABELEDIT:
        {
            CallDefaultProc = false;
            NMLVDISPINFOW* Param = reinterpret_cast<NMLVDISPINFOW*>(lParam);
            std::wstring Text = Param->item.pszText ? Param->item.pszText : L"";
            bool Accept = true;
            OnEndLabelEdit(this, Param->item.iItem, Accept, Text);
            return Accept ? TRUE : FALSE;
        }
        case LVN_COLUMNCLICK:
        {
            NMLISTVIEW* Param = reinterpret_cast<NMLISTVIEW*>(lParam);
            OnColumnClick(this, Param->iSubItem, -1);
            break;
        }
        case LVN_COLUMNDROPDOWN:
        {
            NMLISTVIEW* Param = reinterpret_cast<NMLISTVIEW*>(lParam);
            OnColumnDropDown(this, Param->iSubItem, -1);
            break;
        }
        case LVN_HOTTRACK:
        {
            NMLISTVIEW* Param = reinterpret_cast<NMLISTVIEW*>(lParam);
            OnItemHotTrack(this, Param->iItem, Param->iSubItem - 1);
            break;
        }
        case LVN_ITEMACTIVATE:
        {
            NMITEMACTIVATE* Param = reinterpret_cast<NMITEMACTIVATE*>(lParam);
            OnItemActive(this, Param->iItem, -1);
            break;
        }
        case LVN_ITEMCHANGING:
        {
            NMLISTVIEW* Param = reinterpret_cast<NMLISTVIEW*>(lParam);
            OnItemChanging(this, Param->iItem, -1);
            break;
        }
        case LVN_ITEMCHANGED:
        {
            NMLISTVIEW* Param = reinterpret_cast<NMLISTVIEW*>(lParam);
            OnItemChanged(this, Param->iItem, -1);
            break;
        }
        case LVN_LINKCLICK:
        {
            NMLVLINK* Param = reinterpret_cast<NMLVLINK*>(lParam);
            if (ListView_HasGroup(_Handle, Param->iSubItem))
            {
                OnGroupTaskClick(this, Param->iSubItem, -1);
            }
            break;
        }
        case NM_CLICK:
        {
            NMITEMACTIVATE* Param = reinterpret_cast<NMITEMACTIVATE*>(lParam);
            OnItemClick(this, Param->iItem, Param->iSubItem - 1);
            break;
        }
        case NM_DBLCLK:
        {
            NMITEMACTIVATE* Param = reinterpret_cast<NMITEMACTIVATE*>(lParam);
            OnItemDblClick(this, Param->iItem, Param->iSubItem - 1);
            break;
        }
        case NM_RCLICK:
        {
            NMITEMACTIVATE* Param = reinterpret_cast<NMITEMACTIVATE*>(lParam);
            OnItemRightClick(this, Param->iItem, Param->iSubItem - 1);
            break;
        }
        case NM_RDBLCLK:
        {
            NMITEMACTIVATE* Param = reinterpret_cast<NMITEMACTIVATE*>(lParam);
            OnItemRightDblClick(this, Param->iItem, Param->iSubItem - 1);
            break;
        }
        }
        Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
        break;
    default:
        Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
    }
    return Result;
}
ListViewStyle WinListView::GetViewStyle()
{
    switch (ListView_GetView(_Handle))
    {
    case LV_VIEW_ICON:
        return ListViewStyle::Large;
    case LV_VIEW_SMALLICON:
        return ListViewStyle::Small;
    case LV_VIEW_LIST:
        return ListViewStyle::List;
    case LV_VIEW_DETAILS:
        return ListViewStyle::Report;
    default:
        return ListViewStyle::Tile;
    }
}
void WinListView::SetViewStyle(ListViewStyle Style)
{
    switch (Style)
    {
    case ListViewStyle::Large:
        ListView_SetView(_Handle, LV_VIEW_ICON);
        break;
    case ListViewStyle::Small:
        ListView_SetView(_Handle, LV_VIEW_SMALLICON);
        break;
    case ListViewStyle::List:
        ListView_SetView(_Handle, LV_VIEW_LIST);
        break;
    case ListViewStyle::Report:
        ListView_SetView(_Handle, LV_VIEW_DETAILS);
        break;
    case ListViewStyle::Tile:
        ListView_SetView(_Handle, LV_VIEW_TILE);
        break;
    }
}
void WinListView::Sort(void* Param)
{
    ListViewSortParam P;
    P.Param = Param;
    P.ListView = this;
    ListView_SortItemsEx(_Handle, ListViewCompareFunc, &P);
}
void WinListView::InsertItem(int Index, std::wstring& Text, int ImageIndex)
{
    LVITEMW Item;
    memset(&Item, 0, sizeof(Item));
    Item.mask = LVIF_TEXT | (ImageIndex == -1 ? 0 : LVIF_IMAGE) | LVIF_PARAM;
    Item.pszText = &Text[0];
    Item.cchTextMax = Text.length() + 1;
    Item.iItem = Index;
    Item.iImage = ImageIndex;
    Item.lParam = 0;
    ListView_InsertItem(_Handle, &Item);
}
WinListViewItem WinListView::GetItem(int Index)
{
    if (Index >= 0 && Index < GetItemCount())
    {
        return WinListViewItem(_Handle, Index);
    }
    else
    {
        return WinListViewItem();
    }
}
void WinListView::InsertColumn(int Index, std::wstring& Text, int SubItemIndex)
{
    LVCOLUMNW Column;
    memset(&Column, 0, sizeof(Column));
    Column.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
    Column.pszText = &Text[0];
    Column.cchTextMax = Text.length() + 1;
    Column.iSubItem = SubItemIndex + 1;
    Column.cx = 100;
    if (ListView_InsertColumn(_Handle, Index, &Column) != -1)
    {
        _ColumnCount++;
    }
}
void WinListView::DeleteColumn(int Index)
{
    if (ListView_DeleteColumn(_Handle, Index) == TRUE)
    {
        _ColumnCount--;
    }
}
WinListViewColumn WinListView::GetColumn(int Index)
{
    if (Index >= 0 && Index < GetColumnCount())
    {
        return WinListViewColumn(_Handle, Index);
    }
    else
    {
        return WinListViewColumn();
    }
}
int WinListView::InsertGroup(int Index, std::wstring& Text)
{
    LVGROUP Group;
    memset(&Group, 0, sizeof(Group));
    Group.cbSize = sizeof(Group);
    Group.mask = LVGF_HEADER | LVGF_GROUPID;
    Group.pszHeader = &Text[0];
    Group.iGroupId = _UsedGroupIDs;
    ListView_InsertGroup(_Handle, Index, &Group);
    return _UsedGroupIDs++;
}
void WinListView::DeleteGroup(int Index)
{
    WinListViewGroup Group = GetGroup(Index);
    if (Group)
    {
        ListView_RemoveGroup(_Handle, Group.GetGroupID());
    }
}
WinListViewGroup WinListView::GetGroup(int Index)
{
    if (Index >= 0 && Index < GetGroupCount())
    {
        LVGROUP Group;
        FillListViewGroup(&Group, LVGF_GROUPID);
        ListView_GetGroupInfoByIndex(_Handle, Index, &Group);
        return GetGroupByID(Group.iGroupId);
    }
    else
    {
        return WinListViewGroup();
    }
}

WinListViewGroup WinListView::GetGroupByID(int ID)
{
    if (ListView_HasGroup(_Handle, ID) == TRUE)
    {
        return WinListViewGroup(_Handle, ID);
    }
    else
    {
        return WinListViewGroup();
    }
}
void FillTreeViewInsertItem(TVINSERTSTRUCT* Item, std::wstring& Text, int ImageIndex, int SelectedImageIndex)
{
    if (SelectedImageIndex == -1)
    {
        SelectedImageIndex = ImageIndex;
    }
    memset(Item, 0, sizeof(*Item));
    Item->itemex.mask = TVIF_TEXT | (ImageIndex == -1 ? 0 : TVIF_IMAGE | TVIF_SELECTEDIMAGE);
    if (Text.length())
    {
        Item->itemex.pszText = &Text[0];
    }
    if (ImageIndex != -1)
    {
        Item->itemex.iImage = ImageIndex;
        Item->itemex.iSelectedImage = SelectedImageIndex;
    }
}

void FillTreeViewItem(TVITEMEX* Item, HTREEITEM Handle, UINT Mask)
{
    memset(Item, 0, sizeof(*Item));
    Item->hItem = Handle;
    Item->mask = Mask;
}
WinTreeViewItem::WinTreeViewItem(HWND Handle, HTREEITEM Item)
    : _Handle(Handle)
    , _Item(Item)
{
}
WinTreeViewItem::WinTreeViewItem(const WinTreeViewItem& Item)
    : WinTreeViewItem(Item._Handle, Item._Item)
{
}
WinTreeViewItem::WinTreeViewItem()
    : WinTreeViewItem(0, nullptr)
{
}
WinTreeViewItem& WinTreeViewItem::operator=(const WinTreeViewItem& Item)
{
    _Handle = Item._Handle;
    _Item = Item._Item;
    return *this;
}
WinTreeViewItem WinTreeViewItem::InsertAtLast(std::wstring& Text, int ImageIndex, int SelectedImageIndex)
{
    TVINSERTSTRUCT Item;
    FillTreeViewInsertItem(&Item, Text, ImageIndex, SelectedImageIndex);
    Item.hParent = _Item;
    Item.hInsertAfter = TVI_LAST;
    return WinTreeViewItem(_Handle, TreeView_InsertItem(_Handle, &Item));
}

WinTreeViewItem WinTreeViewItem::InsertAtFirst(std::wstring& Text, int ImageIndex, int SelectedImageIndex)
{
    TVINSERTSTRUCT Item;
    FillTreeViewInsertItem(&Item, Text, ImageIndex, SelectedImageIndex);
    Item.hParent = _Item;
    Item.hInsertAfter = TVI_FIRST;
    return WinTreeViewItem(_Handle, TreeView_InsertItem(_Handle, &Item));
}

WinTreeViewItem WinTreeViewItem::InsertAfter(std::wstring& Text, int ImageIndex, int SelectedImageIndex)
{
    TVINSERTSTRUCT Item;
    FillTreeViewInsertItem(&Item, Text, ImageIndex, SelectedImageIndex);
    Item.hParent = GetParent()._Item;
    Item.hInsertAfter = _Item;
    return WinTreeViewItem(_Handle, TreeView_InsertItem(_Handle, &Item));
}

void WinTreeViewItem::DeleteSelf()
{
    TreeView_DeleteItem(_Handle, _Item);
    _Handle = 0;
    _Item = 0;
}
std::wstring WinTreeViewItem::GetText()
{
    wchar_t Buffer[LISTVIEW_TEXT_BUFFER_SIZE] = {0};
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_TEXT);
    Item.pszText = Buffer;
    Item.cchTextMax = sizeof(Buffer) / sizeof(*Buffer);
    TreeView_GetItem(_Handle, &Item);
    return Buffer;
}

void WinTreeViewItem::SetText(std::wstring& Value)
{
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_TEXT);
    if (Value.length())
    {
        Item.pszText = &Value[0];
    }
    TreeView_SetItem(_Handle, &Item);
}

int WinTreeViewItem::GetImageIndex()
{
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_IMAGE);
    TreeView_GetItem(_Handle, &Item);
    return Item.iImage;
}

void WinTreeViewItem::SetImageIndex(int Value)
{
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_IMAGE);
    Item.iImage = Value;
    TreeView_SetItem(_Handle, &Item);
}

int WinTreeViewItem::GetSelectedImageIndex()
{
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_SELECTEDIMAGE);
    TreeView_GetItem(_Handle, &Item);
    return Item.iSelectedImage;
}

void WinTreeViewItem::SetSelectedImageIndex(int Value)
{
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_SELECTEDIMAGE);
    Item.iSelectedImage = Value;
    TreeView_SetItem(_Handle, &Item);
}

void* WinTreeViewItem::GetCustomData()
{
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_PARAM);
    TreeView_GetItem(_Handle, &Item);
    return (void*)Item.lParam;
}

void WinTreeViewItem::SetCustomData(void* Value)
{
    TVITEMEX Item;
    FillTreeViewItem(&Item, _Item, TVIF_PARAM);
    Item.lParam = (LPARAM)Value;
    TreeView_SetItem(_Handle, &Item);
}
WinTreeView::WinTreeView(WinContainer* Parent)
    : WinControl()
    , _NormalImageList(nullptr)
    , _StateImageList(nullptr)
{
    _CreateWindow(
        TVS_EX_DOUBLEBUFFER,
        WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TVS_DISABLEDRAGDROP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
        WC_TREEVIEW,
        Parent);
    TreeView_SetUnicodeFormat(_Handle, TRUE);
    //SetExStyle(TVS_EX_DOUBLEBUFFER,true);
}

WinTreeView::~WinTreeView()
{
}

LRESULT WinTreeView::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure)
{
    LRESULT Result = 0;
    switch (Message)
    {
    case WM_NOTIFY_DISPATCHED:
        switch ((reinterpret_cast<NMHDR*>(lParam))->code)
        {
        case NM_CLICK:
        {
            OnClick(this);
            break;
        }
        case NM_DBLCLK:
        {
            OnDblClick(this);
            break;
        }
        case NM_RCLICK:
        {
            OnRightClick(this);
            break;
        }
        case NM_RDBLCLK:
        {
            OnRightDblClick(this);
            break;
        }
        case TVN_BEGINDRAG:
        {
            NMTREEVIEWW* Param = reinterpret_cast<NMTREEVIEWW*>(lParam);
            OnBeginDrag(this, WinTreeViewItem(_Handle, Param->itemNew.hItem));
            break;
        }
        case TVN_BEGINRDRAG:
        {
            NMTREEVIEWW* Param = reinterpret_cast<NMTREEVIEWW*>(lParam);
            OnBeginRightDrag(this, WinTreeViewItem(_Handle, Param->itemNew.hItem));
            break;
        }
        case TVN_BEGINLABELEDIT:
        {
            CallDefaultProcedure = false;
            NMTVDISPINFOW* Param = reinterpret_cast<NMTVDISPINFOW*>(lParam);
            bool Accept = true;
            OnBeginLabelEdit(this, WinTreeViewItem(_Handle, Param->item.hItem), Accept, (Param->item.pszText ? Param->item.pszText : L""));
            return Accept ? FALSE : TRUE;
            break;
        }
        case TVN_ENDLABELEDIT:
        {
            CallDefaultProcedure = false;
            NMTVDISPINFOW* Param = reinterpret_cast<NMTVDISPINFOW*>(lParam);
            bool Accept = true;
            OnEndLabelEdit(this, WinTreeViewItem(_Handle, Param->item.hItem), Accept, (Param->item.pszText ? Param->item.pszText : L""));
            return Accept ? TRUE : FALSE;
            break;
        }
        case TVN_ITEMEXPANDING:
        {
            NMTREEVIEWW* Param = reinterpret_cast<NMTREEVIEWW*>(lParam);
            OnItemExpanding(this, WinTreeViewItem(_Handle, Param->itemNew.hItem));
            break;
        }
        case TVN_ITEMEXPANDED:
        {
            NMTREEVIEWW* Param = reinterpret_cast<NMTREEVIEWW*>(lParam);
            OnItemExpanded(this, WinTreeViewItem(_Handle, Param->itemNew.hItem));
            break;
        }
        case TVN_SELCHANGING:
        {
            CallDefaultProcedure = false;
            NMTREEVIEWW* Param = reinterpret_cast<NMTREEVIEWW*>(lParam);
            bool Accept = true;
            OnItemSelecting(this, WinTreeViewItem(_Handle, Param->itemNew.hItem), Accept);
            return Accept ? FALSE : TRUE;
            break;
        }
        case TVN_SELCHANGED:
        {
            NMTREEVIEWW* Param = reinterpret_cast<NMTREEVIEWW*>(lParam);
            OnItemSelected(this, WinTreeViewItem(_Handle, Param->itemNew.hItem));
            break;
        }
        }
        Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProcedure);
        break;
    default:
        Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProcedure);
    }
    return Result;
}

WinTreeViewItem WinTreeView::AddRootItem(std::wstring& Text, int ImageIndex, int SelectedImageIndex)
{
    TVINSERTSTRUCT Item;
    FillTreeViewInsertItem(&Item, Text, ImageIndex, SelectedImageIndex);
    Item.hParent = NULL;
    Item.hInsertAfter = TVI_ROOT;
    return WinTreeViewItem(_Handle, TreeView_InsertItem(_Handle, &Item));
}

}
}
