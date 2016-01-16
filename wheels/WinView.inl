#include "WinView.h"
inline
WinListViewItem::operator bool()
{
    return _Handle != 0;
}
inline
int WinListViewItem::GetIndex()
{
    return _Index;
}
inline
std::wstring WinListViewItem::GetText()
{
    return GetSubItem(-1);
}
inline
void WinListViewItem::SetText(std::wstring& Value)
{
    SetSubItem(-1, Value);
}
inline
bool WinListViewItem::GetCutState()
{
    return ListView_GetItemState(_Handle, _Index, LVIS_CUT) != 0;
}
inline
void WinListViewItem::SetCutState(bool Value)
{
    ListView_SetItemState(_Handle, _Index, (Value ? LVIS_CUT : 0), LVIS_CUT);
}
inline
bool WinListViewItem::GetDropHilitedState()
{
    return ListView_GetItemState(_Handle, _Index, LVIS_DROPHILITED) != 0;
}
inline
void WinListViewItem::SetDropHilitedState(bool Value)
{
    ListView_SetItemState(_Handle, _Index, (Value ? LVIS_DROPHILITED : 0), LVIS_DROPHILITED);
}
inline
bool WinListViewItem::GetFocusedState()
{
    return ListView_GetItemState(_Handle, _Index, LVIS_FOCUSED) != 0;
}
inline
void WinListViewItem::SetFocusedState(bool Value)
{
    ListView_SetItemState(_Handle, _Index, (Value ? LVIS_FOCUSED : 0), LVIS_FOCUSED);
}
inline
bool WinListViewItem::GetSelectedState()
{
    return ListView_GetItemState(_Handle, _Index, LVIS_SELECTED) != 0;
}
inline
void WinListViewItem::SetSelectedState(bool Value)
{
    ListView_SetItemState(_Handle, _Index, (Value ? LVIS_SELECTED : 0), LVIS_SELECTED);
}
inline
bool WinListViewItem::GetChecked()
{
    return ListView_GetCheckState(_Handle, _Index) != 0;
}
inline
void WinListViewItem::SetChecked(bool Value)
{
    ListView_SetCheckState(_Handle, _Index, (Value ? TRUE : FALSE));
}
inline
void WinListViewItem::LeaveGroup()
{
    EnterGroup(I_GROUPIDNONE);
}
inline
bool WinListViewItem::IsInGroup()
{
    return GetOwnerGroupID() != I_GROUPIDNONE;
}
inline
WinListViewColumn::operator bool()
{
    return _Handle != 0;
}
inline
int WinListViewColumn::GetIndex()
{
    return _Index;
}
inline
WinListViewGroup::operator bool()
{
    return _Handle != 0;
}
inline
int WinListViewGroup::GetGroupID()
{
    return _ID;
}
inline
std::wstring WinListViewGroup::GetHeader()
{
    return GetListViewGroupText(LVGF_HEADER, &LVGROUP::pszHeader, &LVGROUP::cchHeader);
}
inline
void WinListViewGroup::SetHeader(std::wstring& Value)
{
    SetListViewGroupText(LVGF_HEADER, &LVGROUP::pszHeader, Value);
}
inline
std::wstring WinListViewGroup::GetFooter()
{
    return GetListViewGroupText(LVGF_FOOTER, &LVGROUP::pszFooter, &LVGROUP::cchFooter);
}
inline
void WinListViewGroup::SetFooter(std::wstring& Value)
{
    SetListViewGroupText(LVGF_FOOTER, &LVGROUP::pszFooter, Value);
}
inline
std::wstring WinListViewGroup::GetSubTitle()
{
    return GetListViewGroupText(LVGF_SUBTITLE, &LVGROUP::pszSubtitle, &LVGROUP::cchSubtitle);
}
inline
void WinListViewGroup::SetSubTitle(std::wstring& Value)
{
    SetListViewGroupText(LVGF_SUBTITLE, &LVGROUP::pszSubtitle, Value);
}
inline
std::wstring WinListViewGroup::GetTask()
{
    return GetListViewGroupText(LVGF_TASK, &LVGROUP::pszTask, &LVGROUP::cchTask);
}
inline
void WinListViewGroup::SetTask(std::wstring& Value)
{
    SetListViewGroupText(LVGF_TASK, &LVGROUP::pszTask, Value);
}
inline
std::wstring WinListViewGroup::GetDescriptionTop()
{
    return GetListViewGroupText(LVGF_DESCRIPTIONTOP, &LVGROUP::pszDescriptionTop, &LVGROUP::cchDescriptionTop);
}
inline
void WinListViewGroup::SetDescriptionTop(std::wstring& Value)
{
    SetListViewGroupText(LVGF_DESCRIPTIONTOP, &LVGROUP::pszDescriptionTop, Value);
}
inline
std::wstring WinListViewGroup::GetDescriptionBottom()
{
    return GetListViewGroupText(LVGF_DESCRIPTIONBOTTOM, &LVGROUP::pszDescriptionBottom, &LVGROUP::cchDescriptionBottom);
}
inline
void WinListViewGroup::SetDescriptionBottom(std::wstring& Value)
{
    SetListViewGroupText(LVGF_DESCRIPTIONBOTTOM, &LVGROUP::pszDescriptionBottom, Value);
}
inline
DWORD WinListView::InternalGetExStyle()
{
    return ListView_GetExtendedListViewStyle(handle);
}
inline
void WinListView::InternalSetExStyle(DWORD ExStyle)
{
    ListView_SetExtendedListViewStyle(handle, ExStyle);
}
inline
WinImageList* WinListView::GetSmallImageList()
{
    return _SmallImageList;
}
inline
void WinListView::SetSmallImageList(WinImageList* ImageList)
{
    ListView_SetImageList(handle, ImageList->GetHandle(), LVSIL_SMALL);
    _SmallImageList = ImageList;
}
inline
WinImageList* WinListView::GetLargeImageList()
{
    return _LargeImageList;
}
inline
void WinListView::SetLargeImageList(WinImageList* ImageList)
{
    ListView_SetImageList(handle, ImageList->GetHandle(), LVSIL_NORMAL);
    _LargeImageList = ImageList;
}
inline
WinImageList* WinListView::GetStateImageList()
{
    return _StateImageList;
}
inline
void WinListView::SetStateImageList(WinImageList* ImageList)
{
    ListView_SetImageList(handle, ImageList->GetHandle(), LVSIL_STATE);
    _StateImageList = ImageList;
}
inline
WinImageList* WinListView::GetGroupImageList()
{
    return _GroupImageList;
}
inline
void WinListView::SetGroupImageList(WinImageList* ImageList)
{
    ListView_SetImageList(handle, ImageList->GetHandle(), LVSIL_GROUPHEADER);
    _GroupImageList = ImageList;
}
inline
COLORREF WinListView::GetOutlineColor()
{
    return ListView_GetOutlineColor(handle);
}
inline
void WinListView::SetOutlineColor(COLORREF Value)
{
    ListView_SetOutlineColor(handle, Value);
}
inline
COLORREF WinListView::GetTextColor()
{
    return ListView_GetTextColor(handle);
}
inline
void WinListView::SetTextColor(COLORREF Value)
{
    ListView_SetTextColor(handle, Value);
}
inline
COLORREF WinListView::GetTextBackgroundColor()
{
    return ListView_GetTextBkColor(handle);
}
inline
void WinListView::SetTextBackgroundColor(COLORREF Value)
{
    ListView_SetTextBkColor(handle, Value);
}
inline
COLORREF WinListView::GetBackgroundColor()
{
    return ListView_GetBkColor(handle);
}
inline
void WinListView::SetBackgroundColor(COLORREF Value)
{
    ListView_SetBkColor(handle, Value);
}
inline
bool WinListView::GetAutoArrange()
{
    return GetStyle(LVS_AUTOARRANGE);
}
inline
void WinListView::SetAutoArrange(bool Value)
{
    SetStyle(LVS_AUTOARRANGE, Value);
}
inline
bool WinListView::GetEditable()
{
    return GetStyle(LVS_EDITLABELS);
}
inline
void WinListView::SetEditable(bool Value)
{
    SetStyle(LVS_EDITLABELS, Value);
}
inline
bool WinListView::GetColumnPushable()
{
    return !GetStyle(LVS_NOSORTHEADER);
}
inline
void WinListView::SetColumnPushable(bool Value)
{
    SetStyle(LVS_NOSORTHEADER, !Value);
}
inline
bool WinListView::GetAlwaysShowSelection()
{
    return GetStyle(LVS_SHOWSELALWAYS);
}
inline
void WinListView::SetAlwaysShowSelection(bool Value)
{
    SetStyle(LVS_SHOWSELALWAYS, Value);
}
inline
bool WinListView::GetSingleSelection()
{
    return GetStyle(LVS_SINGLESEL);
}
inline
void WinListView::SetSingleSelection(bool Value)
{
    SetStyle(LVS_SINGLESEL, Value);
}
inline
bool WinListView::GetShowCheckBoxes()
{
    return GetExStyle(LVS_EX_CHECKBOXES);
}
inline
void WinListView::SetShowCheckBoxes(bool Value)
{
    SetExStyle(LVS_EX_CHECKBOXES, Value);
}
inline
bool WinListView::GetAutoCheckSelect()
{
    return GetExStyle(LVS_EX_AUTOCHECKSELECT);
}
inline
void WinListView::SetAutoCheckSelect(bool Value)
{
    SetExStyle(LVS_EX_AUTOCHECKSELECT, Value);
}
inline
bool WinListView::GetBorderSelect()
{
    return GetExStyle(LVS_EX_BORDERSELECT);
}
inline
void WinListView::SetBorderSelect(bool Value)
{
    SetExStyle(LVS_EX_BORDERSELECT, Value);
}
inline
bool WinListView::GetShowGridLines()
{
    return GetExStyle(LVS_EX_GRIDLINES);
}
inline
void WinListView::SetShowGridLines(bool Value)
{
    SetExStyle(LVS_EX_GRIDLINES, Value);
}
inline
bool WinListView::GetHeaderDragable()
{
    return GetExStyle(LVS_EX_HEADERDRAGDROP);
}
inline
void WinListView::SetHeaderDragable(bool Value)
{
    SetExStyle(LVS_EX_HEADERDRAGDROP, Value);
}
inline
bool WinListView::GetOneClickActive()
{
    return GetExStyle(LVS_EX_ONECLICKACTIVATE);
}
inline
void WinListView::SetOneClickActive(bool Value)
{
    SetExStyle(LVS_EX_ONECLICKACTIVATE, Value);
}
inline
bool WinListView::GetTwoClickActive()
{
    return GetExStyle(LVS_EX_TWOCLICKACTIVATE);
}
inline
void WinListView::SetTwoClickActive(bool Value)
{
    SetExStyle(LVS_EX_TWOCLICKACTIVATE, Value);
}
inline
bool WinListView::GetUnderlineHot()
{
    return GetExStyle(LVS_EX_UNDERLINEHOT);
}
inline
void WinListView::SetUnderlineHot(bool Value)
{
    SetExStyle(LVS_EX_UNDERLINEHOT, Value);
}
inline
bool WinListView::GetUnderlineCold()
{
    return GetExStyle(LVS_EX_UNDERLINECOLD);
}
inline
void WinListView::SetUnderlineCold(bool Value)
{
    SetExStyle(LVS_EX_UNDERLINECOLD, Value);
}
inline
bool WinListView::GetAutoAutoArrange()
{
    return GetExStyle(LVS_EX_AUTOAUTOARRANGE);
}
inline
void WinListView::SetAutoAutoArrange(bool Value)
{
    SetExStyle(LVS_EX_AUTOAUTOARRANGE, Value);
}
inline
bool WinListView::GetSnapToGrid()
{
    return GetExStyle(LVS_EX_SNAPTOGRID);
}
inline
void WinListView::SetSnapToGrid(bool Value)
{
    SetExStyle(LVS_EX_SNAPTOGRID, Value);
}
inline
void WinListView::EditLabel(int Index)
{
    ListView_EditLabel(handle, Index);
}
inline
void WinListView::CancelEditLabel()
{
    ListView_CancelEditLabel(handle);
}
inline
void WinListView::Arrange()
{
    ListView_Arrange(handle, LVA_DEFAULT);
}
inline
void WinListView::SnapToGrid()
{
    ListView_Arrange(handle, LVA_SNAPTOGRID);
}
inline
void WinListView::EnsureItemVisible(int Index, bool AcceptPartialVisible)
{
    ListView_EnsureVisible(handle, Index, (AcceptPartialVisible ? TRUE : FALSE));
}
inline
int WinListView::GetItemCount()
{
    return ListView_GetItemCount(handle);
}
inline
void WinListView::AddItem(std::wstring& Text, int ImageIndex)
{
    InsertItem(GetItemCount(), Text, ImageIndex);
}
inline
void WinListView::DeleteItem(int Index)
{
    ListView_DeleteItem(handle, Index);
}
inline
void WinListView::Clear()
{
    ListView_DeleteAllItems(handle);
}
inline
int WinListView::GetSelectedItemCount()
{
    return ListView_GetSelectedCount(handle);
}
inline
int WinListView::GetSelectionItemMark()
{
    return ListView_GetSelectionMark(handle);
}
inline
void WinListView::SetSelectionItemMark(int Value)
{
    ListView_SetSelectionMark(handle, Value);
}
inline
int WinListView::GetHotItemIndex()
{
    return ListView_GetHotItem(handle);
}
inline
void WinListView::SetHotItemIndex(int Value)
{
    ListView_SetHotItem(handle, Value);
}
inline
int WinListView::GetColumnCount()
{
    return _ColumnCount;
}
inline
void WinListView::AddColumn(std::wstring& Text, int SubItemIndex)
{
    InsertColumn(GetColumnCount(), Text, SubItemIndex);
}
inline
int WinListView::GetSelectedColumnIndex()
{
    return ListView_GetSelectedColumn(handle);
}
inline
void WinListView::SetSelectedColumnIndex(int Value)
{
    ListView_SetSelectedColumn(handle, Value);
}
inline
void WinListView::EnableGrouping(bool Value)
{
    ListView_EnableGroupView(handle, (Value ? TRUE : FALSE));
}
inline
int WinListView::GetGroupCount()
{
    return ListView_GetGroupCount(handle);
}
inline
int WinListView::AddGroup(std::wstring& Text)
{
    return InsertGroup(-1, Text);
}
inline
void WinListView::DeleteGroupByID(int ID)
{
    ListView_RemoveGroup(handle, ID);
}
inline
WinTreeViewItem::operator bool()
{
    return _Item != 0;
}
inline
WinTreeViewItem WinTreeViewItem::GetParent()
{
    return WinTreeViewItem(_Handle, TreeView_GetParent(_Handle, _Item));
}
inline
WinTreeViewItem WinTreeViewItem::GetFirstChild()
{
    return WinTreeViewItem(_Handle, TreeView_GetChild(_Handle, _Item));
}
inline
WinTreeViewItem WinTreeViewItem::GetPrevious()
{
    return WinTreeViewItem(_Handle, TreeView_GetPrevSibling(_Handle, _Item));
}
inline
WinTreeViewItem WinTreeViewItem::GetNext()
{
    return WinTreeViewItem(_Handle, TreeView_GetNextSibling(_Handle, _Item));
}
inline
WinTreeViewItem WinTreeViewItem::GetNextVisible()
{
    return WinTreeViewItem(_Handle, TreeView_GetNextVisible(_Handle, _Item));
}
inline
WinTreeViewItem WinTreeViewItem::GetPrevVisible()
{
    return WinTreeViewItem(_Handle, TreeView_GetPrevVisible(_Handle, _Item));
}
inline
WinTreeViewItem WinTreeViewItem::GetNextSelected()
{
    return WinTreeViewItem(_Handle, TreeView_GetNextSelected(_Handle, _Item));
}
inline
HTREEITEM WinTreeViewItem::GetHandle()
{
    return _Item;
}
inline
void WinTreeViewItem::SetDropTarget()
{
    TreeView_SelectDropTarget(_Handle, _Item);
}
inline
void WinTreeViewItem::SetFirstVisible()
{
    TreeView_SelectSetFirstVisible(_Handle, _Item);
}
inline
bool WinTreeViewItem::GetBoldState()
{
    return TreeView_GetItemState(_Handle, _Item, TVIS_BOLD) != 0;
}
inline
void WinTreeViewItem::SetBoldState(bool Value)
{
    TreeView_SetItemState(_Handle, _Item, (Value ? TVIS_BOLD : 0), TVIS_BOLD);
}
inline
bool WinTreeViewItem::GetCutState()
{
    return TreeView_GetItemState(_Handle, _Item, TVIS_CUT) != 0;
}
inline
void WinTreeViewItem::SetCutState(bool Value)
{
    TreeView_SetItemState(_Handle, _Item, (Value ? TVIS_CUT : 0), TVIS_CUT);
}
inline
bool WinTreeViewItem::GetDropHilitedState()
{
    return TreeView_GetItemState(_Handle, _Item, TVIS_DROPHILITED) != 0;
}
inline
void WinTreeViewItem::SetDropHilitedState(bool Value)
{
    TreeView_SetItemState(_Handle, _Item, (Value ? TVIS_DROPHILITED : 0), TVIS_DROPHILITED);
}
inline
bool WinTreeViewItem::GetExpendedState()
{
    return TreeView_GetItemState(_Handle, _Item, TVIS_EXPANDED) != 0;
}
inline
void WinTreeViewItem::SetExpendedState(bool Value)
{
    TreeView_SetItemState(_Handle, _Item, (Value ? TVIS_EXPANDED : 0), TVIS_EXPANDED);
}
inline
bool WinTreeViewItem::GetSelectedState()
{
    return TreeView_GetItemState(_Handle, _Item, TVIS_SELECTED) != 0;
}
inline
void WinTreeViewItem::SetSelectedState(bool Value)
{
    TreeView_SetItemState(_Handle, _Item, (Value ? TVIS_SELECTED : 0), TVIS_SELECTED);
}
inline
bool WinTreeViewItem::GetChecked()
{
    return TreeView_GetCheckState(_Handle, _Item) == TRUE;
}
inline
void WinTreeViewItem::SetChecked(bool Value)
{
    TreeView_SetCheckState(_Handle, _Item, (Value ? TRUE : FALSE));
}
inline
void WinTreeViewItem::EnsureVisible()
{
    TreeView_EnsureVisible(_Handle, _Item);
}
inline
void WinTreeViewItem::Expand()
{
    TreeView_Expand(_Handle, _Item, TVE_EXPAND);
}
inline
void WinTreeViewItem::Collapse()
{
    TreeView_Expand(_Handle, _Item, TVE_COLLAPSE);
}
inline
void WinTreeViewItem::Toggle()
{
    TreeView_Expand(_Handle, _Item, TVE_TOGGLE);
}
inline
WinImageList* WinTreeView::GetNormalImageList()
{
    return _NormalImageList;
}
inline
void WinTreeView::SetNormalImageList(WinImageList* ImageList)
{
    TreeView_SetImageList(handle, ImageList->GetHandle(), TVSIL_NORMAL);
    _NormalImageList = ImageList;
}
inline
WinImageList* WinTreeView::GetStateImageList()
{
    return _StateImageList;
}
inline
void WinTreeView::SetStateImageList(WinImageList* ImageList)
{
    TreeView_SetImageList(handle, ImageList->GetHandle(), TVSIL_STATE);
    _StateImageList = ImageList;
}
inline
COLORREF WinTreeView::GetLineColor()
{
    return TreeView_GetLineColor(handle);
}
inline
void WinTreeView::SetLineColor(COLORREF Value)
{
    TreeView_SetLineColor(handle, Value);
}
inline
COLORREF WinTreeView::GetTextColor()
{
    return TreeView_GetTextColor(handle);
}
inline
void WinTreeView::SetTextColor(COLORREF Value)
{
    TreeView_SetTextColor(handle, Value);
}
inline
COLORREF WinTreeView::GetBackColor()
{
    return TreeView_GetBkColor(handle);
}
inline
void WinTreeView::SetBackColor(COLORREF Value)
{
    TreeView_SetBkColor(handle, Value);
}
inline
bool WinTreeView::GetShowCheckBoxes()
{
    return GetStyle(TVS_CHECKBOXES);
}
inline
void WinTreeView::SetShowCheckBoxes(bool Value)
{
    SetStyle(TVS_CHECKBOXES, Value);
}
inline
bool WinTreeView::GetEnableDragDrop()
{
    return !GetStyle(TVS_DISABLEDRAGDROP);
}
inline
void WinTreeView::SetEnableDragDrop(bool Value)
{
    SetStyle(TVS_DISABLEDRAGDROP, !Value);
}
inline
bool WinTreeView::GetEditable()
{
    return GetStyle(TVS_EDITLABELS);
}
inline
void WinTreeView::SetEditable(bool Value)
{
    SetStyle(TVS_EDITLABELS, Value);
}
inline
bool WinTreeView::GetFullRowSelect()
{
    return GetStyle(TVS_FULLROWSELECT);
}
inline
void WinTreeView::SetFullRowSelect(bool Value)
{
    SetStyle(TVS_FULLROWSELECT, Value);
}
inline
bool WinTreeView::GetHasButtons()
{
    return GetStyle(TVS_HASBUTTONS);
}
inline
void WinTreeView::SetHasButtons(bool Value)
{
    SetStyle(TVS_HASBUTTONS, Value);
}
inline
bool WinTreeView::GetHasLines()
{
    return GetStyle(TVS_HASLINES);
}
inline
void WinTreeView::SetHasLines(bool Value)
{
    SetStyle(TVS_HASLINES, Value);
}
inline
bool WinTreeView::GetAlwaysShowSelection()
{
    return GetStyle(TVS_SHOWSELALWAYS);
}
inline
void WinTreeView::SetAlwaysShowSelection(bool Value)
{
    SetStyle(TVS_SHOWSELALWAYS, Value);
}
inline
bool WinTreeView::GetDimmedCheckBoxes()
{
    return GetExStyle(TVS_EX_DIMMEDCHECKBOXES);
}
inline
void WinTreeView::SetDimmedCheckBoxes(bool Value)
{
    SetExStyle(TVS_EX_DIMMEDCHECKBOXES, Value);
}
inline
bool WinTreeView::GetExclusionCheckBoxes()
{
    return GetExStyle(TVS_EX_EXCLUSIONCHECKBOXES);
}
inline
void WinTreeView::SetExclusionCheckBoxes(bool Value)
{
    SetExStyle(TVS_EX_EXCLUSIONCHECKBOXES, Value);
}
inline
bool WinTreeView::GetFadeButton()
{
    return GetExStyle(TVS_EX_FADEINOUTEXPANDOS);
}
inline
void WinTreeView::SetFadeButton(bool Value)
{
    SetExStyle(TVS_EX_FADEINOUTEXPANDOS, Value);
}
inline
void WinTreeView::EditLabel(WinTreeViewItem Item)
{
    TreeView_EditLabel(handle, Item._Item);
}
inline
void WinTreeView::FinishEditLabel()
{
    TreeView_EndEditLabelNow(handle, FALSE);
}
inline
void WinTreeView::CancelEditLabel()
{
    TreeView_EndEditLabelNow(handle, TRUE);
}
inline
WinTreeViewItem WinTreeView::GetFirstRoot()
{
    return WinTreeViewItem(handle, TreeView_GetRoot(handle));
}
inline
WinTreeViewItem WinTreeView::GetCurrentSelectedItem()
{
    return WinTreeViewItem(handle, TreeView_GetSelection(handle));
}
inline
WinTreeViewItem WinTreeView::GetFirstVisible()
{
    return WinTreeViewItem(handle, TreeView_GetFirstVisible(handle));
}
inline
WinTreeViewItem WinTreeView::GetLastVisible()
{
    return WinTreeViewItem(handle, TreeView_GetLastVisible(handle));
}
inline
WinTreeViewItem WinTreeView::GetDropHighlighted()
{
    return WinTreeViewItem(handle, TreeView_GetDropHilight(handle));
}
inline
void WinTreeView::Clear()
{
    TreeView_DeleteAllItems(handle);
}
