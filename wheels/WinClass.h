#pragma once
#ifndef WINCLASS_H
#define WINCLASS_H

#include "base.h"
#include <vector>
#include <string>
#include "Event.h"
#include "Pointer.h"
#include <map>

#include "WinGDI.h"
#include <windows.h>
#include <CommCtrl.h>

#define WM_GETPOINTER_DISPATCHED	(WM_USER + 1)
#define WM_COMMAND_DISPATCHED		(WM_USER + 2)
#define WM_NOTIFY_DISPATCHED		(WM_USER + 3)
#define WM_VSCROLL_DISPATCHED		(WM_USER + 4)
#define WM_HSCROLL_DISPATCHED		(WM_USER + 5)
#define WM_USER_PLGUI				(WM_USER + 6)

namespace pl
{
namespace windows
{

class WinClass: public Object
{
protected:
    std::wstring _Name;
    WNDCLASSEX _WindowClass;
    ATOM _ClassAtom;
public:
    WinClass(const std::wstring& Name, bool Shadow, bool OwnDC, WNDPROC Procedure, HINSTANCE hInstance);

    bool IsAvailable();
    std::wstring GetName();
    ATOM GetClassAtom();
};

enum class WinHAlign
{
    Left, Center, Right
};

enum class WinVAlign
{
    Left, Center, Bottom
};

struct MouseStruct
{
    int Wheel;
    bool Ctrl;
    bool Shift;
    bool LeftButton;
    bool MiddleButton;
    bool RightButton;
    int X, Y;

    MouseStruct(WPARAM wParam, LPARAM lParam, bool WheelMessage);
};

struct KeyStruct
{
    int KeyCode;
    int RepeatCount;
    bool PreviousDown;
    bool Alt;

    KeyStruct(WPARAM wParam, LPARAM lParam);
};

using NotifyEvent = Event<Object*>;
using MovingEvent = Event<Object*, LPRECT>;
using SizingEvent = Event<Object*, bool, bool, bool, bool, LPRECT>;
using QueryEvent = Event<Object*, bool&>;
using MouseEvent = Event<Object*, MouseStruct>;
using KeyEvent = Event<Object*, KeyStruct>;
using CharEvent = Event<Object*, wchar_t&>;
using HotKeyEvent = Event<Object*, int>;
using DropDownEvent = Event<Object*, RECT>;

extern int KeyCodeFromName(const std::wstring& Name);

class IAcceleratorReceiver: public Interface
{
    friend class AcceleratorItem;
private:
    AcceleratorItem* _Item;
protected:
    virtual void OnAttachAcceleratorItem() = 0;
    virtual void OnDetachAcceleratorItem() = 0;
    virtual void OnExecuteAcceleratorItem() = 0;
    AcceleratorItem* GetAcceleratorItem();
public:
    IAcceleratorReceiver();
    ~IAcceleratorReceiver();

    bool Bind(AcceleratorItem* Item);
    void Unbind();
};

class AcceleratorItem: public Object
{
protected:
    IAcceleratorReceiver* _Receiver;
    std::wstring _Name;
public:
    AcceleratorItem(const std::wstring& Name);
    ~AcceleratorItem();

    bool Bind(IAcceleratorReceiver* Receiver);
    void Unbind();
    void Execute();
    std::wstring GetName();
};

typedef std::vector<std::shared_ptr<AcceleratorItem>> AcceleratorItemList;

enum AcceleratorKey: int
{
    None = 0,
    Ctrl = 1,
    Shift = 2,
    Alt = 4
};

class AcceleratorManager: public Object
{
protected:
    std::vector<ACCEL> _AcceleratorList;
    AcceleratorItemList _AcceleratorItems;
    HACCEL _Handle;
public:
    AcceleratorManager();
    ~AcceleratorManager();

    HACCEL GetHandle();
    AcceleratorItem* GetAcceleratorItem(AcceleratorKey SysKeys, std::wstring& Key);
    AcceleratorItem* GetAcceleratorItem(int ID);
};

extern AcceleratorManager* GetAcceleratorManager();
extern AcceleratorItem* Accel(AcceleratorKey SysKeys, std::wstring& key);

class WinControl;
class WinContainer;

enum class PlacementDirection
{
    Vertical, Horizontal
};

enum class PlacementBehavior
{
    FixPlacement1, FixPlacement2, FixScale
};

class Placement: public Object
{
    friend class WinContainer;
private:
    Placement();
protected:
    WinContainer* _Container;
    Placement* _Parent;
    WinControl* _Control;
    Placement* _Placement1;
    Placement* _Placement2;
    int _SpliterPosition;
    PlacementDirection _SpliterDirection;
    int _SpliterSize;
    double _SpliterScale;
    PlacementBehavior _Behavior;
    int _BorderSize;
    int _MinClientWidth;
    int _MinClientHeight;
    bool _DisableResize;

    Placement(Placement* Parent);
    Placement(WinContainer* Container);
    ~Placement();

    void OnSizing(Object* Sender, bool Left, bool Right, bool Top, bool Bottom, LPRECT Area);
    void OnSized(Object* Sender);
    //void ModifySize(RECT Area);
    void Resize();
    void UnregisterControl(WinControl* Control);
public:
    int GetSpliterPosition();
    void SetSpliterPosition(int Position);

    PlacementDirection GetSpliterDirection();
    void SetSpliterDirection(PlacementDirection Dircetion);

    PlacementBehavior GetBehavior();
    void SetBehavior(PlacementBehavior Behavior);

    int GetBorderSize();
    void SetBorderSize(int Value);

    int GetSpliterSize();
    void SetSpliterSize(int Value);

    double GetSpliterScale();
    void SetSpliterScale(double Scale);

    WinControl* GetControl();
    void SetControl(WinControl* Control);

    int GetMinClientWidth();
    void SetMinClientWidth(int Value);

    int GetMinClientHeight();
    void SetMinClientHeight(int Value);

    Placement* GetPlacement1();
    Placement* GetPlacement2();

    RECT GetArea();
    RECT GetMinSize();

    int GetPlacement1Position();
    void SetPlacement1Position(int Value);

    int GetPlacement2Position();
    void SetPlacement2Position(int Value);
};

namespace placement
{

class Base: public Interface
{
public:
    using Pointer = std::shared_ptr<Base>;
    virtual void Apply(Placement* Place) = 0;
};

class Blank
{
protected:
    int _MinWidth;
    int _MinHeight;
public:
    Blank(int MinWidth, int MinHeight);

    operator Base::Pointer();
};

class Control
{
protected:
    WinControl* _Control;
    int _Border;
    int _MinWidth;
    int _MinHeight;
public:
    Control(WinControl* Control);
    Control(WinControl* Control, int border);
    Control(WinControl* Control, int MinWidth, int MinHeight);
    Control(WinControl* Control, int Border, int MinWidth, int MinHeight);

    operator Base::Pointer();
};

class VertFix1
{
protected:
    int _Border;
    int _SpliterSize;
    Base::Pointer _Placement1;
    Base::Pointer _Placement2;
public:
    VertFix1(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2);

    operator Base::Pointer();
};

class VertFix2
{
protected:
    int _Border;
    int _SpliterSize;
    Base::Pointer _Placement1;
    Base::Pointer _Placement2;
public:
    VertFix2(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2);

    operator Base::Pointer();
};

class VertScale
{
protected:
    int _Border;
    int _SpliterSize;
    double _Scale;
    Base::Pointer _Placement1;
    Base::Pointer _Placement2;
public:
    VertScale(int Border, int SpliterSize, double Scale, Base::Pointer Placement1, Base::Pointer Placement2);

    operator Base::Pointer();
};

class HorzFix1
{
protected:
    int _Border;
    int _SpliterSize;
    Base::Pointer _Placement1;
    Base::Pointer _Placement2;
public:
    HorzFix1(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2);

    operator Base::Pointer();
};

class HorzFix2
{
protected:
    int _Border;
    int _SpliterSize;
    Base::Pointer _Placement1;
    Base::Pointer _Placement2;
public:
    HorzFix2(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2);

    operator Base::Pointer();
};

class HorzScale
{
protected:
    int _Border;
    int _SpliterSize;
    double _Scale;
    Base::Pointer _Placement1;
    Base::Pointer _Placement2;
public:
    HorzScale(int Border, int SpliterSize, double Scale, Base::Pointer Placement1, Base::Pointer Placement2);

    operator Base::Pointer();
};

} //namespace placement

class WinControl: public Object
{
protected:
    HWND handle;
    bool isMouseEntered;
    bool isHoverOnce;
    bool isEnableHover;
    int hoverTime;
    int lastX;
    int lastY;
    WinContainer* parent;
    WinFont::Ptr font;
    bool isSubClass;

    bool InternalCreateWindow(DWORD ExStyle, DWORD Style, const wchar_t* ClassName, WinContainer* Parent);

    virtual DWORD InternalGetExStyle();
    virtual void InternalSetExStyle(DWORD ExStyle);
    bool GetExStyle(DWORD ExStyle);
    void SetExStyle(DWORD ExStyle, bool Available);
    bool GetStyle(DWORD Style);
    void SetStyle(DWORD Style, bool Available);
    void TrackMouse(bool Enable, int HoverTime);
    void Destroy();
    void GetWindowPosSize(int& l, int& t, int& w, int& h);
    void SetWindowPosSize(int& l, int& t, int& w, int& h);
public:
    NotifyEvent OnEnabled;
    NotifyEvent OnDisabled;
    NotifyEvent OnMoved;
    MovingEvent OnMoving;
    NotifyEvent OnShow;
    NotifyEvent OnHide;
    NotifyEvent OnSized;
    SizingEvent OnSizing;
    NotifyEvent OnGotFocus;
    NotifyEvent OnLostFocus;
    NotifyEvent OnPaint;

    MouseEvent OnMouseMove;
    MouseEvent OnLeftButtonDown;
    MouseEvent OnLeftButtonUp;
    MouseEvent OnLeftButtonDblClick;
    MouseEvent OnRightButtonDown;
    MouseEvent OnRightButtonUp;
    MouseEvent OnRightButtonDblClick;
    MouseEvent OnMiddleButtonDown;
    MouseEvent OnMiddleButtonUp;
    MouseEvent OnMiddleButtonDblClick;
    MouseEvent OnMouseHWheel;
    MouseEvent OnMouseVWheel;
    MouseEvent OnMouseHover;
    NotifyEvent OnMouseEnter;
    NotifyEvent OnMouseLeave;

    KeyEvent OnKeyDown;
    KeyEvent OnKeyUp;
    KeyEvent OnSysKeyDown;
    KeyEvent OnSysKeyUp;
    CharEvent OnChar;

    WinControl();
    WinControl(DWORD exStyle, DWORD style, const wchar_t* className, WinContainer* parent);
    ~WinControl();

    virtual LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure);

    HWND GetHandle();

    WinContainer* GetParent();
    void SetParent(WinContainer* Parent);

    bool GetEnableHover();
    void SetEnableHover(bool Value);

    bool GetHoverOnce();
    void SetHoverOnce(bool Value);

    int GetHoverTime();
    void SetHoverTime(int Value);

    int GetLeft();
    void SetLeft(int Value);

    int GetTop();
    void SetTop(int Value);

    int GetWidth();
    void SetWidth(int Value);

    int GetHeight();
    void SetHeight(int Value);

    void Move(int Left, int Top, int Width, int Height);

    std::wstring GetText();
    void SetText(const std::wstring& Value);

    bool GetEnabled();
    void SetEnabled(bool Value);

    bool GetVisible();
    void SetVisible(bool Value);
    
    bool GetFocused();
    void SetFocused();

    bool GetAcceptFiles();
    void SetAcceptFiles(bool Value);

    WinFont::Ptr GetFont();
    void SetFont(WinFont::Ptr Font);
};
typedef std::vector<WinControl*> WinControlList;

class WinContainer: public WinControl
{
    friend class WinControl;
protected:
    WinControlList controls;
    Placement* placement;

    void RegisterChild(WinControl* Control);
    void UnregisterChild(WinControl* Control);
    void Destroy();
    virtual void GetClientArea(RECT* Rect);
public:
    WinContainer();
    WinContainer(DWORD exStyle, DWORD style, const wchar_t* className, WinContainer* parent);
    ~WinContainer();

    Placement* GetPlacement();
    void ApplyPlacement(placement::Base::Pointer Placement);

    int GetControlCount();
    WinControl* GetControl(int Index);

    bool GetClipChildren();
    void SetClipChildren(bool Value);

    bool GetClipSiblings();
    void SetClipSiblings(bool Value);

    int GetClientWidth();
    void SetClientWidth(int Value);

    int GetClientHeight();
    void SetClientHeight(int Value);

    WinControlList::iterator begin();
    WinControlList::iterator end();
};

class WinTimer;
class WinMenu;
class WinMenuItem;
class WinFormMenu;

typedef std::vector<SharedPtr<WinTimer>> WinTimerList;
typedef std::vector<SharedPtr<WinMenu>> WinMenuList;
typedef std::map<int, WinMenuItem*> WinMenuItemMap;

enum class WinFormBorder
{
    None, Border, Single, Sizable
};

class WinForm: public WinContainer
{
    friend WinTimer;
    friend WinMenu;
    friend WinMenuItem;
protected:
    bool isMainForm;
    bool hasCreated;

    std::vector<int> hotKeys;
    WinTimerList timerList;
    WinMenuList menuList;
    WinFormMenu* formMenu;
    WinMenuItemMap menuItemMap;
    int usedMenuItemHandle;
    std::vector<int> freeMenuItemHandles;

    int RegisterOwned(WinTimer* Timer);
    void UnregisterOwned(WinTimer* Timer);
    void RegisterOwned(WinMenu* Menu);
    void UnregisterOwned(WinMenu* Menu);

    int GenerateMenuItemHandle();
    void RegisterMenuItem(int Handle, WinMenuItem* Item);
    void UnregisterMenuItem(int Handle);

    virtual void InitializeComponents();

    void CreateForm();
public:
    NotifyEvent OnActivateApp;
    NotifyEvent OnDeactivateApp;
    NotifyEvent OnActivate;
    NotifyEvent OnDeactivate;
    QueryEvent OnQueryClose;
    NotifyEvent OnClose;
    NotifyEvent OnCompacting;
    NotifyEvent OnCreate;
    NotifyEvent OnDestroy;
    NotifyEvent OnEnterSizeMove;
    NotifyEvent OnLeaveSizeMove;
    HotKeyEvent OnHotKey;

    WinForm(bool Create);
    WinForm();
    ~WinForm();

    LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;
    bool RegisterGlobalHotKey(int ID, bool Alt, bool Ctrl, bool Shift, bool Win, int KeyCode);
    void UnregisterGlobalHotKey(int ID);
    WinMenuItem* MenuItemO_Handle(int Handle);

    WinFormBorder GetBorder();
    void SetBorder(WinFormBorder Value);

    bool GetMaximizeBox();
    void SetMaximizeBox(bool value);

    bool GetMinimizeBox();
    void SetMinimizeBox(bool Value);

    bool GetIconAvailable();
    void SetIconAvailable(bool Value);

    bool GetAppearInTaskBar();
    void SetAppearInTaskBar(bool Value);

    bool GetTopMost();
    void SetTopMost(bool Value);

    bool GetTransparent();
    void SetTransparent(bool Value);

    bool GetWindowEdge();
    void SetWindowEdge(bool Value);

    bool GetToolWindow();
    void SetToolWindow(bool Value);

    bool GetActived();
    void SetActived();

    void Show();
    void ShowRestored();
    void ShowMaximized();
    void ShowMinimized();
    void ShowModal();
    void MoveCenter();
    void Close();
    void DestroyForm();

    WinFormMenu* GetFormMenu();
    void SetFormMenu(WinFormMenu* Value);
};

class WinTimer: public Object
{
protected:
    WinForm* owner;
    UINT_PTR handle;
    int interval;
    bool enabled;
public:
    NotifyEvent OnTimer;

    WinTimer(WinForm* Owner);
    ~WinTimer();

    UINT_PTR GetHandle();
    WinForm* GetOwner();

    int GetInterval();
    void SetInterval(int Value);

    bool GetEnabled();
    void SetEnabled(bool Value);
};

class WinImageList: public Object
{
protected:
    HIMAGELIST handle;
    WinBitmap::Ptr CheckBitmap(WinBitmap::Ptr Bitmap);
    //WinBitmap::Pointer CreateMask(WinBitmap::Pointer Bitmap);
public:
    WinImageList(int Width, int Height);
    WinImageList(WinImageList* ImageList);
    ~WinImageList();

    HIMAGELIST GetHandle();
    void Add(WinBitmap::Ptr Bitmap);
    void Replace(int Index, WinBitmap::Ptr Bitmap);
    void Remove(int Index);
    int GetCount();
    int GetImageWidth();
    int GetImageHeight();
    COLORREF GetBackgroundColor();
    void SetBackgroundColor(COLORREF Value);
    bool GetUseBackgroundColor();
    void SetUseBackgroundColor(bool Value);
    void Draw(WinDC* DC, int Index, int X, int Y, UINT Style);
};

class WinPopupMenu;

using WinMenuItemList = std::vector<WinMenuItem*>;

class WinMenu: public Object
{
    friend class WinMenuItem;
protected:
    HMENU handle;
    WinForm* owner;
    WinForm* messageDispatcher;
    bool isAssociated;
    WinMenuItemList menItems;
    WinMenuItem* associatedMenuItems;

    virtual void ItemChanged();
public:
    WinMenu(WinForm* Owner);
    ~WinMenu();

    HMENU GetHandle();
    int GetCount();
    WinMenuItem* GetItem(int Index);
    bool GetAssociated();
    WinMenuItem* GetAssociatedMenuItem();

    WinMenuItem* Append();
    WinMenuItem* Insert(int index);
    void Delete(int Index);
    void Delete(WinMenuItem* MenuItem);

    WinMenuItemList::iterator begin();
    WinMenuItemList::iterator end();
};

class WinMenuItem: public Object, public IAcceleratorReceiver
{
    friend WinMenu;
protected:
    WinMenu* owner;
    int handle;
    WinPopupMenu* subMenu;
    WinBitmap::Ptr checkedBitmap;
    WinBitmap::Ptr uncheckedBitmap;

    bool isChecked;
    bool isSeparator;
    bool isRadioCheck;
    int radioGroup;
    bool isEnabled;
    bool isHighlighted;
    std::wstring text;
    std::wstring menuText;

    void OnAttachAcceleratorItem() override;
    void OnDetachAcceleratorItem() override;
    void OnExecuteAcceleratorItem() override;
    void FillStruct(MENUITEMINFO* Info);
    void RefreshProperties();
    WinBitmap::Ptr CheckBitmap(WinBitmap::Ptr Bitmap);
public:
    NotifyEvent OnClick;
    NotifyEvent OnRightClick;

    WinMenuItem(WinMenu* Owner, int Handle);
    ~WinMenuItem();

    WinMenu* GetOwner();

    bool GetChecked();
    void SetChecked(bool Value);

    bool GetSeparator();
    void SetSeparator(bool Value);

    bool GetRadioCheck();
    void SetRadioCheck(bool Value);

    int GetRadioGroup();
    void SetRadioGroup(int Value);

    bool GetEnabled();
    void SetEnabled(bool Value);

    bool GetHighlighted();
    void SetHighlighted(bool Value);

    std::wstring GetText();
    void SetText(const std::wstring& Text);

    WinBitmap::Ptr GetCheckedBitmap();
    void SetCheckedBitmap(WinBitmap::Ptr Bitmap);

    WinBitmap::Ptr GetUncheckedBitmap();
    void SetUncheckedBitmap(WinBitmap::Ptr Bitmap);

    WinPopupMenu* GetSubMenu();
    WinPopupMenu* CreateSubMenu();
    void DestroySubMenu();
};

class WinFormMenu: public WinMenu
{
    friend class WinForm;
protected:
    WinForm* parent;
    void ItemChanged() override;
public:
    WinFormMenu(WinForm* Owner);
    ~WinFormMenu();
};

class WinPopupMenu: public WinMenu
{
public:
    WinPopupMenu(WinForm* Owner);
    ~WinPopupMenu();

    WinMenuItem* Popup(int X, int Y, bool KeepPrevMenu = false);
    WinMenuItem* Popup(bool KeepPrevMenu = false);
};

enum class MsgBoxButton : UINT
{
    AbortRetryIgnore,
    OK,
    OKCancel,
    RetryCancel,
    YesNo,
    YesNoCancel
};

enum class MsgBoxIcon
{
    None,
    Exclamation,
    Information,
    Question,
    Error
};

enum class MsgBoxDefaultButton
{
    Button1, Button2, Button3, Button4
};

enum class MsgBoxResult
{
    Abort, Retry, Ignore, OK, Cancel, Yes, No
};

extern MsgBoxResult WinMsgBox(
    WinForm* Caller,
    const std::wstring& Text,
    const std::wstring& Caption,
    MsgBoxButton Button,
    MsgBoxIcon Icon = MsgBoxIcon::None,
    MsgBoxDefaultButton DefaultButton = MsgBoxDefaultButton::Button1
    );
extern void ShowMessage(WinForm* Caller, const std::wstring& Message);

enum class Align
{
    LeftTop, Center, RightBottom
};

#include "WinClass.inl"

} //namespace windows
} //namespace pl



#endif // !WINCLASS_H
