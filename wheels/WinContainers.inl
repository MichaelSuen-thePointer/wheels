inline
int WinTabPage::GetIndex()
{
	return _Index;
}

inline
Placement* WinTabPage::GetPlacement()
{
	return _Container->GetPlacement();
}

inline
void WinTabPage::ApplyPlacement(placement::Base::Pointer Placement)
{
	_Container->ApplyPlacement(Placement);
}

inline
int WinTabPage::GetControlCount()
{
	return _Container->GetControlCount();
}

inline
WinControl* WinTabPage::GetControl(int Index)
{
	return _Container->GetControl(Index);
}

inline
int WinTabPage::GetClientWidth()
{
	return _Container->GetClientWidth();
}

inline
int WinTabPage::GetClientHeight()
{
	return _Container->GetClientHeight();
}

inline
WinControlList::iterator pl::windows::WinTabPage::begin()
{
	return _Container->begin();
}

inline
WinControlList::iterator pl::windows::WinTabPage::end()
{
	return _Container->end();
}

inline
WinImageList* WinTab::GetImageList()
{
	return _ImageList;
}

inline
void WinTab::SetImageList(WinImageList* ImageList)
{
	TabCtrl_SetImageList(_Handle, ImageList->GetHandle());
	_ImageList = ImageList;
}

inline
bool WinTab::GetButtonTab()
{
	return GetStyle(TCS_BUTTONS);
}

inline
void WinTab::SetButtonTab(bool Value)
{
	SetStyle(TCS_BUTTONS, Value);
}

inline
bool WinTab::GetFlatButton()
{
	return GetStyle(TCS_FLATBUTTONS);
}

inline
void WinTab::SetFlatButton(bool Value)
{
	SetStyle(TCS_FLATBUTTONS, Value);
}

inline
bool WinTab::GetFlatSeparator()
{
	return GetExStyle(TCS_EX_FLATSEPARATORS);
}

inline
void WinTab::SetFlatSeparator(bool Value)
{
	return SetExStyle(TCS_EX_FLATSEPARATORS, Value);
}

inline
bool WinTab::GetMultiline()
{
	return GetStyle(TCS_MULTILINE);
}

inline
void WinTab::SetMultiline(bool Value)
{
	SetStyle(TCS_MULTILINE, Value);
	ArrangeTabContainers();
}

inline
bool WinTab::GetTabAtLeftTop()
{
	return !GetStyle(TCS_BOTTOM);
}

inline
void WinTab::SetTabAtLeftTop(bool Value)
{
	SetStyle(TCS_BOTTOM, !Value);
	ArrangeTabContainers();
}

inline
bool WinTab::GetVerticalTab()
{
	return GetStyle(TCS_VERTICAL);
}

inline
void WinTab::SetVerticalTab(bool Value)
{
	SetStyle(TCS_VERTICAL, Value);
	ArrangeTabContainers();
}

inline
WinTabPage WinTab::AddPage(std::wstring& Text, WinContainer* Container)
{
	return InsertPage(GetPageCount(), Text, Container);
}

inline
int WinTab::GetPageCount()
{
	assert(TabCtrl_GetItemCount(_Handle) == _TabContainers.size());
	return TabCtrl_GetItemCount(_Handle);
}

inline
int WinTab::GetRowCount()
{
	return TabCtrl_GetRowCount(_Handle);
}
