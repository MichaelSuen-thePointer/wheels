#include "WinClass.h"
extern AcceleratorManager* _AcceleratorManager;

inline
AcceleratorManager* GetAcceleratorManager()
{
	return _AcceleratorManager;
}

inline
AcceleratorItem* Accel(AcceleratorKey SysKeys, std::wstring& Key)
{
	return GetAcceleratorManager()->GetAcceleratorItem(SysKeys, Key);
}

inline
AcceleratorItem* IAcceleratorReceiver::GetAcceleratorItem()
{
	return _Item;
}

inline
IAcceleratorReceiver::IAcceleratorReceiver()
	: _Item(nullptr)
{}

inline
IAcceleratorReceiver::~IAcceleratorReceiver()
{
	if (_Item)
	{
		_Item->Unbind();
	}
}

inline
void AcceleratorItem::Unbind()
{
	_Receiver = nullptr;
}

inline
void AcceleratorItem::Execute()
{
	if (_Receiver)
	{
		_Receiver->OnExecuteAcceleratorItem();
	}
}

inline
std::wstring AcceleratorItem::GetName()
{
	return _Name;
}

inline
HACCEL AcceleratorManager::GetHandle()
{
	return _Handle;
}

inline
int Placement::GetSpliterPosition()
{
	return _SpliterPosition;
}

inline
void Placement::SetSpliterPosition(int Value)
{
	_SpliterPosition = Value;
	Resize();
}

inline
PlacementDirection Placement::GetSpliterDirection()
{
	return _SpliterDirection;
}

inline
void Placement::SetSpliterDirection(PlacementDirection Value)
{
	_SpliterDirection = Value;
	Resize();
}

inline
PlacementBehavior Placement::GetBehavior()
{
	return _Behavior;
}

inline
void Placement::SetBehavior(PlacementBehavior Behavior)
{
	_Behavior = Behavior;
}

inline
int Placement::GetBorderSize()
{
	return _BorderSize;
}

inline
void Placement::SetBorderSize(int Value)
{
	_BorderSize = Value;
	Resize();
}

inline
int Placement::GetSpliterSize()
{
	return _SpliterSize;
}

inline
void Placement::SetSpliterSize(int Value)
{
	_SpliterSize = Value;
	Resize();
}

inline
double Placement::GetSpliterScale()
{
	return _SpliterScale;
}

inline
void Placement::SetSpliterScale(double Value)
{
	_SpliterScale = Value;
	Resize();
}

inline
WinControl* Placement::GetControl()
{
	return _Control;
}

inline
void Placement::SetControl(WinControl* Value)
{
	_Control = Value;
	Resize();
}

inline
int Placement::GetMinClientWidth()
{
	return _MinClientHeight;
}

inline
void Placement::SetMinClientWidth(int Value)
{
	_MinClientHeight = Value;
	Resize();
}

inline
int Placement::GetMinClientHeight()
{
	return _MinClientHeight;
}

inline
void Placement::SetMinClientHeight(int Value)
{
	_MinClientHeight = Value;
	Resize();
}

inline
bool WinClass::IsAvailable()
{
	return _ClassAtom != 0;
}

inline
std::wstring WinClass::GetName()
{
	if (IsAvailable())
	{
		return _Name;
	}
	else
	{
		return L"";
	}
}

inline
ATOM WinClass::GetClassAtom()
{
	return _ClassAtom;
}

inline
void WinControl::SetWindowPosSize(int& Left, int& Top, int& Width, int& Height)
{
	MoveWindow(_Handle, Left, Top, Width, Height, TRUE);
}

inline
HWND WinControl::GetHandle()
{
	return _Handle;
}

inline
WinContainer* WinControl::GetParent()
{
	return _Parent;
}

inline
bool WinControl::GetEnableHover()
{
	return _EnableHover;
}

inline
void WinControl::SetEnableHover(bool Value)
{
	_EnableHover = Value;
	if (_EnableHover)
	{
		TrackMouse(true, _HoverTime);
	}
}

inline
bool WinControl::GetHoverOnce()
{
	return _HoverOnce;
}

inline
void WinControl::SetHoverOnce(bool Value)
{
	_HoverOnce = Value;
	if (!_HoverOnce && _EnableHover)
	{
		TrackMouse(true, _HoverTime);
	}
}

inline
int WinControl::GetHoverTime()
{
	return _HoverTime;
}

inline
void WinControl::SetHoverTime(int Value)
{
	if (Value >= 0)
	{
		_HoverTime = Value;
	}
	else
	{
		_HoverTime = -1;
	}
}

inline
bool WinControl::GetEnabled()
{
	return IsWindowEnabled(_Handle) != 0;
}

inline
void WinControl::SetEnabled(bool Value)
{
	EnableWindow(_Handle, Value ? TRUE : FALSE);
}

inline
bool WinControl::GetVisible()
{
	return IsWindowVisible(_Handle) != 0;
}

inline
void WinControl::SetVisible(bool Value)
{
	ShowWindow(_Handle, Value ? SW_SHOW : SW_HIDE);
}

inline
bool WinControl::GetFocused()
{
	return GetFocus() == _Handle;
}

inline
void WinControl::SetFocused()
{
	SetFocus(_Handle);
}

inline
bool WinControl::GetAcceptFiles()
{
	return GetExStyle(WS_EX_ACCEPTFILES);
}

inline
void WinControl::SetAcceptFiles(bool Value)
{
	SetExStyle(WS_EX_ACCEPTFILES, Value);
}

inline
WinFont::Pointer WinControl::GetFont()
{
	return _Font;
}

inline
void WinControl::SetFont(WinFont::Pointer Font)
{
	_Font = Font;
	SendMessage(_Handle, WM_SETFONT, reinterpret_cast<WPARAM>(_Font->GetHandle()), TRUE);
}

inline
void WinContainer::GetClientArea(RECT* Rect)
{
	GetClientRect(_Handle, Rect);
}

inline
Placement* WinContainer::GetPlacement()
{
	return _Placement ? _Placement : _Placement = new Placement(this);
}

inline
void WinContainer::ApplyPlacement(placement::Base::Pointer Placement)
{
	Placement->Apply(GetPlacement());
}

inline
int WinContainer::GetControlCount()
{
	return _Controls.size();
}

inline
WinControl* WinContainer::GetControl(int Index)
{
	return _Controls[Index];
}

inline
bool WinContainer::GetClipChildren()
{
	return GetStyle(WS_CLIPCHILDREN);
}

inline
void WinContainer::SetClipChildren(bool Value)
{
	SetStyle(WS_CLIPCHILDREN, Value);
}

inline
bool WinContainer::GetClipSiblings()
{
	return GetStyle(WS_CLIPSIBLINGS);
}

inline
void WinContainer::SetClipSiblings(bool Value)
{
	SetStyle(WS_CLIPSIBLINGS, Value);
}

inline
int WinContainer::GetClientWidth()
{
	RECT Rect;
	GetClientArea(&Rect);
	return Rect.right;
}

inline
void WinContainer::SetClientWidth(int Value)
{
	SetWidth(Value + GetWidth() - GetClientWidth());
}

inline
int WinContainer::GetClientHeight()
{
	RECT Rect;
	GetClientArea(&Rect);
	return Rect.bottom;
}

inline
void WinContainer::SetClientHeight(int Value)
{
	SetHeight(Value + GetHeight() - GetClientHeight());
}

inline
WinControlList::iterator pl::windows::WinContainer::begin()
{
	return _Controls.begin();
}

inline
WinControlList::iterator pl::windows::WinContainer::end()
{
	return _Controls.end();
}

inline
void WinForm::RegisterMenuItem(int Handle, WinMenuItem* Item)
{
	_MenuItemMap.insert({Handle, Item});
}

inline
void WinForm::UnregisterMenuItem(int Handle)
{
	_MenuItemMap.erase(Handle);
	_FreeMenuItemHandles.push_back(Handle);
}

inline
void WinForm::InitializeComponents()
{
	//empty
}

inline 
bool WinForm::GetMaximizeBox()
{
	return GetStyle(WS_MAXIMIZEBOX);
}

inline
void WinForm::SetMaximizeBox(bool Value)
{
	SetStyle(WS_MAXIMIZEBOX, Value);
}

inline
bool WinForm::GetMinimizeBox()
{
	return GetStyle(WS_MINIMIZEBOX);
}

inline
void WinForm::SetMinimizeBox(bool Value)
{
	SetStyle(WS_MINIMIZEBOX, Value);
}

inline
bool WinForm::GetIconAvailable()
{
	return GetStyle(WS_SYSMENU);
}

inline
void WinForm::SetIconAvailable(bool Value)
{
	SetStyle(WS_SYSMENU, Value);
}

inline
bool WinForm::GetAppearInTaskBar()
{
	return GetExStyle(WS_EX_APPWINDOW);
}

inline
void WinForm::SetAppearInTaskBar(bool Value)
{
	SetExStyle(WS_EX_APPWINDOW, Value);
}

inline
bool WinForm::GetTopMost()
{
	return GetExStyle(WS_EX_TOPMOST);
}

inline
void WinForm::SetTopMost(bool Value)
{
	SetExStyle(WS_EX_TOPMOST, Value);
	SetWindowPos(_Handle, Value ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);
}

inline
bool WinForm::GetTransparent()
{
	return GetExStyle(WS_EX_TRANSPARENT);
}

inline
void WinForm::SetTransparent(bool Value)
{
	SetExStyle(WS_EX_TRANSPARENT, Value);
}

inline
bool WinForm::GetWindowEdge()
{
	return GetExStyle(WS_EX_WINDOWEDGE);
}

inline
void WinForm::SetWindowEdge(bool Value)
{
	SetExStyle(WS_EX_WINDOWEDGE, Value);
}

inline
bool WinForm::GetToolWindow()
{
	return GetExStyle(WS_EX_TOOLWINDOW);
}

inline
void WinForm::SetToolWindow(bool Value)
{
	SetExStyle(WS_EX_TOOLWINDOW, Value);
}

inline
bool WinForm::GetActived()
{
	return GetActiveWindow() == _Handle;
}

inline
void WinForm::SetActived()
{
	SetActiveWindow(_Handle);
}

inline
void WinForm::Show()
{
	ShowWindow(_Handle, SW_SHOWNORMAL);
}

inline
void WinForm::ShowRestored()
{
	ShowWindow(_Handle, SW_RESTORE);
}

inline
void WinForm::ShowMinimized()
{
	ShowWindow(_Handle, SW_SHOWMINIMIZED);
}

inline
void WinForm::ShowMaximized()
{
	ShowWindow(_Handle, SW_SHOWMAXIMIZED);
}

inline
void WinForm::Close()
{
	CloseWindow(_Handle);
}

inline
void WinForm::DestroyForm()
{
	Destroy();
}

inline
WinFormMenu* WinForm::GetFormMenu()
{
	return _FormMenu;
}

inline
UINT_PTR WinTimer::GetHandle()
{
	return _Handle;
}

inline
WinForm* WinTimer::GetOwner()
{
	return _Owner;
}

inline
int WinTimer::GetInterval()
{
	return _Interval;
}

inline 
bool WinTimer::GetEnabled()
{
	return _Enabled;
}

inline
HIMAGELIST WinImageList::GetHandle()
{
	return _Handle;
}

inline
void WinImageList::Remove(int Index)
{
	ImageList_Remove(_Handle, Index);
}

inline
int WinImageList::GetCount()
{
	return ImageList_GetImageCount(_Handle);
}

inline
int WinImageList::GetImageWidth()
{
	int X, Y;
	ImageList_GetIconSize(_Handle, &X, &Y);
	return Y;
}

inline
int WinImageList::GetImageHeight()
{
	int X, Y;
	ImageList_GetIconSize(_Handle, &X, &Y);
	return Y;
}

inline
COLORREF WinImageList::GetBackgroundColor()
{
	COLORREF Color = ImageList_GetBkColor(_Handle);
	if (Color == CLR_NONE)
	{
		Color = 0;
	}
	return Color;
}

inline
void WinImageList::SetBackgroundColor(COLORREF Color)
{
	ImageList_SetBkColor(_Handle, Color);
}

inline
bool WinImageList::GetUseBackgroundColor()
{
	return ImageList_GetBkColor(_Handle) != CLR_NONE;
}

inline
void WinImageList::SetUseBackgroundColor(bool Value)
{
	if (Value != GetUseBackgroundColor())
	{
		ImageList_SetBkColor(_Handle, Value ? 0 : CLR_NONE);
	}
}

inline
void WinImageList::Draw(WinDC* DC, int Index, int X, int Y, UINT Style)
{
	ImageList_Draw(_Handle, Index, DC->GetHandle(), X, Y, Style);
}

inline
HMENU WinMenu::GetHandle()
{
	return _Handle;
}

inline
int WinMenu::GetCount()
{
	return _MenuItems.size();
}

inline
WinMenuItem* WinMenu::GetItem(int Index)
{
	return _MenuItems[Index];
}

inline
bool WinMenu::GetAssociated()
{
	return _Associated;
}

inline
WinMenuItem* WinMenu::GetAssociatedMenuItem()
{
	return _AssociatedMenuItem;
}

inline
WinMenuItem* WinMenu::Append()
{
	return Insert(_MenuItems.size());
}

inline
WinMenuItemList::iterator WinMenu::begin()
{
	return _MenuItems.begin();
}

inline
WinMenuItemList::iterator WinMenu::end()
{
	return _MenuItems.end();
}

inline
WinMenu* WinMenuItem::GetOwner()
{
	return _Owner;
}

inline
bool WinMenuItem::GetChecked()
{
	return _Checked;
}

inline
bool WinMenuItem::GetSeparator()
{
	return _Separator;
}

inline
void WinMenuItem::SetSeparator(bool Value)
{
	_Separator = Value;
	RefreshProperties();
}

inline
bool WinMenuItem::GetRadioCheck()
{
	return _RadioCheck;
}

inline
void WinMenuItem::SetRadioCheck(bool Value)
{
	_RadioCheck = Value;
	RefreshProperties();
}

inline
int WinMenuItem::GetRadioGroup()
{
	return _RadioGroup;
}

inline
void WinMenuItem::SetRadioGroup(int Value)
{
	_RadioGroup = Value;
	RefreshProperties();
}

inline
bool WinMenuItem::GetEnabled()
{
	return _Enabled;
}

inline
void WinMenuItem::SetEnabled(bool Value)
{
	_Enabled = Value;
	RefreshProperties();
}

inline
bool WinMenuItem::GetHighlighted()
{
	return _Highlighted;
}

inline
void WinMenuItem::SetHighlighted(bool Value)
{
	_Highlighted = Value;
	RefreshProperties();
}

inline
std::wstring WinMenuItem::GetText()
{
	return _Text;
}

inline
void WinMenuItem::SetText(const std::wstring& Text)
{
	_Text = Text;
	RefreshProperties();
}

inline
WinBitmap::Pointer WinMenuItem::GetCheckedBitmap()
{
	return _CheckedBitmap;
}

inline
void WinMenuItem::SetCheckedBitmap(WinBitmap::Pointer Bitmap)
{
	_CheckedBitmap = CheckBitmap(Bitmap);
	RefreshProperties();
}

inline
WinBitmap::Pointer WinMenuItem::GetUncheckedBitmap()
{
	return _UncheckedBitmap;
}

inline
void WinMenuItem::SetUncheckedBitmap(WinBitmap::Pointer Bitmap)
{
	_UncheckedBitmap = CheckBitmap(Bitmap);
	RefreshProperties();
}

inline
WinPopupMenu* WinMenuItem::GetSubMenu()
{
	return _SubMenu;
}