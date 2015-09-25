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
    return ListView_GetExtendedListViewStyle(_Handle);
}
inline
void WinListView::InternalSetExStyle(DWORD ExStyle)
{
    ListView_SetExtendedListViewStyle(_Handle, ExStyle);
}
WinImageList* WinListView::GetSmallImageList()
{
    return _SmallImageList;
}
inline
void WinListView::SetSmallImageList(WinImageList* ImageList)
{
    ListView_SetImageList(_Handle, ImageList->GetHandle(), LVSIL_SMALL);
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
    ListView_SetImageList(_Handle, ImageList->GetHandle(), LVSIL_NORMAL);
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
    ListView_SetImageList(_Handle, ImageList->GetHandle(), LVSIL_STATE);
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
    ListView_SetImageList(_Handle, ImageList->GetHandle(), LVSIL_GROUPHEADER);
    _GroupImageList = ImageList;
}
inline
COLORREF WinListView::GetOutlineColor()
{
    return ListView_GetOutlineColor(_Handle);
}
inline
void WinListView::SetOutlineColor(COLORREF Value)
{
    ListView_SetOutlineColor(_Handle, Value);
}
inline
COLORREF WinListView::GetTextColor()
{
    return ListView_GetTextColor(_Handle);
}

void WinListView::SetTextColor(COLORREF Value)
{
    ListView_SetTextColor(_Handle, Value);
}
inline
COLORREF WinListView::GetTextBackgroundColor()
{
    return ListView_GetTextBkColor(_Handle);
}
inline
void WinListView::SetTextBackgroundColor(COLORREF Value)
{
    ListView_SetTextBkColor(_Handle, Value);
}
inline
COLORREF WinListView::GetBackgroundColor()
{
    return ListView_GetBkColor(_Handle);
}
inline
void WinListView::SetBackgroundColor(COLORREF Value)
{
    ListView_SetBkColor(_Handle, Value);
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
    ListView_EditLabel(_Handle, Index);
}
inline
void WinListView::CancelEditLabel()
{
    ListView_CancelEditLabel(_Handle);
}
inline
void WinListView::Arrange()
{
    ListView_Arrange(_Handle, LVA_DEFAULT);
}
inline
void WinListView::SnapToGrid()
{
    ListView_Arrange(_Handle, LVA_SNAPTOGRID);
}
inline
void WinListView::EnsureItemVisible(int Index, bool AcceptPartialVisible)
{
    ListView_EnsureVisible(_Handle, Index, (AcceptPartialVisible ? TRUE : FALSE));
}
inline
int WinListView::GetItemCount()
{
    return ListView_GetItemCount(_Handle);
}
inline
void WinListView::AddItem(const std::wstring& Text, int ImageIndex)
{
    InsertItem(GetItemCount(), Text, ImageIndex);
}

