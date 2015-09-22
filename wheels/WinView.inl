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
void WinListViewItem::SetText(const std::wstring& Value)
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