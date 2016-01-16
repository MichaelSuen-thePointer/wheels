
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
    MoveWindow(handle, Left, Top, Width, Height, TRUE);
}

inline
HWND WinControl::GetHandle()
{
    return handle;
}

inline
WinContainer* WinControl::GetParent()
{
    return parent;
}

inline
bool WinControl::GetEnableHover()
{
    return isEnableHover;
}

inline
void WinControl::SetEnableHover(bool Value)
{
    isEnableHover = Value;
    if (isEnableHover)
    {
        TrackMouse(true, hoverTime);
    }
}

inline
bool WinControl::GetHoverOnce()
{
    return isHoverOnce;
}

inline
void WinControl::SetHoverOnce(bool Value)
{
    isHoverOnce = Value;
    if (!isHoverOnce && isEnableHover)
    {
        TrackMouse(true, hoverTime);
    }
}

inline
int WinControl::GetHoverTime()
{
    return hoverTime;
}

inline
void WinControl::SetHoverTime(int Value)
{
    if (Value >= 0)
    {
        hoverTime = Value;
    }
    else
    {
        hoverTime = -1;
    }
}

inline
bool WinControl::GetEnabled()
{
    return IsWindowEnabled(handle) != 0;
}

inline
void WinControl::SetEnabled(bool Value)
{
    EnableWindow(handle, Value ? TRUE : FALSE);
}

inline
bool WinControl::GetVisible()
{
    return IsWindowVisible(handle) != 0;
}

inline
void WinControl::SetVisible(bool Value)
{
    ShowWindow(handle, Value ? SW_SHOW : SW_HIDE);
}

inline
bool WinControl::GetFocused()
{
    return GetFocus() == handle;
}

inline
void WinControl::SetFocused()
{
    SetFocus(handle);
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
WinFont::Ptr WinControl::GetFont()
{
    return font;
}

inline
void WinControl::SetFont(WinFont::Ptr Font)
{
    font = Font;
    SendMessageW(handle, WM_SETFONT, reinterpret_cast<WPARAM>(font->GetHandle()), TRUE);
}

inline
void WinContainer::GetClientArea(RECT* Rect)
{
    GetClientRect(handle, Rect);
}

inline
Placement* WinContainer::GetPlacement()
{
    if (!placement)
    {
        placement = new Placement(this);
    }
    return placement;
}

inline
void WinContainer::ApplyPlacement(placement::Base::Pointer Placement)
{
    Placement->Apply(GetPlacement());
}

inline
int WinContainer::GetControlCount()
{
    return controls.size();
}

inline
WinControl* WinContainer::GetControl(int Index)
{
    return controls[Index];
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
    return controls.begin();
}

inline
WinControlList::iterator pl::windows::WinContainer::end()
{
    return controls.end();
}

inline
void WinForm::RegisterMenuItem(int Handle, WinMenuItem* Item)
{
    menuItemMap.insert({Handle, Item});
}

inline
void WinForm::UnregisterMenuItem(int Handle)
{
    menuItemMap.erase(Handle);
    freeMenuItemHandles.push_back(Handle);
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
    SetWindowPos(handle, Value ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);
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
    return GetActiveWindow() == handle;
}

inline
void WinForm::SetActived()
{
    SetActiveWindow(handle);
}

inline
void WinForm::Show()
{
    ShowWindow(handle, SW_SHOWNORMAL);
}

inline
void WinForm::ShowRestored()
{
    ShowWindow(handle, SW_RESTORE);
}

inline
void WinForm::ShowMinimized()
{
    ShowWindow(handle, SW_SHOWMINIMIZED);
}

inline
void WinForm::ShowMaximized()
{
    ShowWindow(handle, SW_SHOWMAXIMIZED);
}

inline
void WinForm::Close()
{
    CloseWindow(handle);
}

inline
void WinForm::DestroyForm()
{
    Destroy();
}

inline
WinFormMenu* WinForm::GetFormMenu()
{
    return formMenu;
}

inline
UINT_PTR WinTimer::GetHandle()
{
    return handle;
}

inline
WinForm* WinTimer::GetOwner()
{
    return owner;
}

inline
int WinTimer::GetInterval()
{
    return interval;
}

inline 
bool WinTimer::GetEnabled()
{
    return enabled;
}

inline
HIMAGELIST WinImageList::GetHandle()
{
    return handle;
}

inline
void WinImageList::Remove(int Index)
{
    ImageList_Remove(handle, Index);
}

inline
int WinImageList::GetCount()
{
    return ImageList_GetImageCount(handle);
}

inline
int WinImageList::GetImageWidth()
{
    int X, Y;
    ImageList_GetIconSize(handle, &X, &Y);
    return Y;
}

inline
int WinImageList::GetImageHeight()
{
    int X, Y;
    ImageList_GetIconSize(handle, &X, &Y);
    return Y;
}

inline
COLORREF WinImageList::GetBackgroundColor()
{
    COLORREF Color = ImageList_GetBkColor(handle);
    if (Color == CLR_NONE)
    {
        Color = 0;
    }
    return Color;
}

inline
void WinImageList::SetBackgroundColor(COLORREF Color)
{
    ImageList_SetBkColor(handle, Color);
}

inline
bool WinImageList::GetUseBackgroundColor()
{
    return ImageList_GetBkColor(handle) != CLR_NONE;
}

inline
void WinImageList::SetUseBackgroundColor(bool Value)
{
    if (Value != GetUseBackgroundColor())
    {
        ImageList_SetBkColor(handle, Value ? 0 : CLR_NONE);
    }
}

inline
void WinImageList::Draw(WinDC* DC, int Index, int X, int Y, UINT Style)
{
    ImageList_Draw(handle, Index, DC->GetHandle(), X, Y, Style);
}

inline
HMENU WinMenu::GetHandle()
{
    return handle;
}

inline
int WinMenu::GetCount()
{
    return menItems.size();
}

inline
WinMenuItem* WinMenu::GetItem(int Index)
{
    return menItems[Index];
}

inline
bool WinMenu::GetAssociated()
{
    return isAssociated;
}

inline
WinMenuItem* WinMenu::GetAssociatedMenuItem()
{
    return associatedMenuItems;
}

inline
WinMenuItem* WinMenu::Append()
{
    return Insert(menItems.size());
}

inline
WinMenuItemList::iterator WinMenu::begin()
{
    return menItems.begin();
}

inline
WinMenuItemList::iterator WinMenu::end()
{
    return menItems.end();
}

inline
WinMenu* WinMenuItem::GetOwner()
{
    return owner;
}

inline
bool WinMenuItem::GetChecked()
{
    return isChecked;
}

inline
bool WinMenuItem::GetSeparator()
{
    return isSeparator;
}

inline
void WinMenuItem::SetSeparator(bool Value)
{
    isSeparator = Value;
    RefreshProperties();
}

inline
bool WinMenuItem::GetRadioCheck()
{
    return isRadioCheck;
}

inline
void WinMenuItem::SetRadioCheck(bool Value)
{
    isRadioCheck = Value;
    RefreshProperties();
}

inline
int WinMenuItem::GetRadioGroup()
{
    return radioGroup;
}

inline
void WinMenuItem::SetRadioGroup(int Value)
{
    radioGroup = Value;
    RefreshProperties();
}

inline
bool WinMenuItem::GetEnabled()
{
    return isEnabled;
}

inline
void WinMenuItem::SetEnabled(bool Value)
{
    isEnabled = Value;
    RefreshProperties();
}

inline
bool WinMenuItem::GetHighlighted()
{
    return isHighlighted;
}

inline
void WinMenuItem::SetHighlighted(bool Value)
{
    isHighlighted = Value;
    RefreshProperties();
}

inline
std::wstring WinMenuItem::GetText()
{
    return text;
}

inline
void WinMenuItem::SetText(const std::wstring& Text)
{
    text = Text;
    RefreshProperties();
}

inline
WinBitmap::Ptr WinMenuItem::GetCheckedBitmap()
{
    return checkedBitmap;
}

inline
void WinMenuItem::SetCheckedBitmap(WinBitmap::Ptr Bitmap)
{
    checkedBitmap = CheckBitmap(Bitmap);
    RefreshProperties();
}

inline
WinBitmap::Ptr WinMenuItem::GetUncheckedBitmap()
{
    return uncheckedBitmap;
}

inline
void WinMenuItem::SetUncheckedBitmap(WinBitmap::Ptr Bitmap)
{
    uncheckedBitmap = CheckBitmap(Bitmap);
    RefreshProperties();
}

inline
WinPopupMenu* WinMenuItem::GetSubMenu()
{
    return subMenu;
}