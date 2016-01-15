#include "WinMain.h"
#include <CommCtrl.h>
#include <cassert>
namespace pl
{
namespace windows
{

MouseStruct::MouseStruct(WPARAM wParam, LPARAM lParam, bool WheelMessage)
    : Wheel(WheelMessage ? GET_WHEEL_DELTA_WPARAM(wParam) : 0)
    , Ctrl(((wParam = GET_KEYSTATE_WPARAM(wParam)) & MK_CONTROL) == 1)
    , Shift((wParam & MK_SHIFT) == 1)
    , LeftButton((wParam & MK_LBUTTON) == 1)
    , MiddleButton((wParam & MK_MBUTTON) == 1)
    , RightButton((wParam & MK_RBUTTON) == 1)
    , X(MAKEPOINTS(lParam).x)
    , Y(MAKEPOINTS(lParam).y)
{
}

KeyStruct::KeyStruct(WPARAM wParam, LPARAM lParam)
    : KeyCode(static_cast<int>(wParam))
    , RepeatCount(lParam % 65536)
    , PreviousDown(((lParam >> 30) & 2) == 1)
    , Alt(((lParam >> 29) & 1) == 1)
{
}


IMPLEMENT_VOID_EVENT(NotifyEvent, (Sender), (Object* Sender))
IMPLEMENT_VOID_EVENT(MovingEvent, (Sender, Area), (Object* Sender, LPRECT Area))
IMPLEMENT_VOID_EVENT(SizingEvent, (Sender, Left, Right, Top, Bottom, Area), (Object* Sender, bool Left, bool Right, bool Top, bool Bottom, LPRECT Area))
IMPLEMENT_VOID_EVENT(QueryEvent, (Sender, Cancel), (Object* Sender, bool& Cancel))
IMPLEMENT_VOID_EVENT(MouseEvent, (Sender, Mouse), (Object* Sender, MouseStruct Mouse))
IMPLEMENT_VOID_EVENT(KeyEvent, (Sender, Key), (Object* Sender, KeyStruct Key))
IMPLEMENT_VOID_EVENT(CharEvent, (Sender, Char), (Object* Sender, wchar_t& Char))
IMPLEMENT_VOID_EVENT(HotKeyEvent, (Sender, ID), (Object* Sender, int ID))
IMPLEMENT_VOID_EVENT(DropDownEvent, (Sender, Rect), (Object* Sender, RECT Rect))

struct KeyNamePair
{
    int Code;
    std::wstring Name;
    KeyNamePair(int _Code, const std::wstring& _Name);
    KeyNamePair(std::pair<int, const std::wstring&>& Pair);
};

KeyNamePair::KeyNamePair(int Code, const std::wstring& Name)
    : Code(Code)
    , Name(Name)
{
}

KeyNamePair::KeyNamePair(std::pair<int, const std::wstring&>& Pair)
    : Code(Pair.first)
    , Name(Pair.second)
{
}

std::vector<KeyNamePair> _AcceleratorNames;
AcceleratorManager* _AcceleratorManager = nullptr;

inline
void AddAcceleratorName(int Code, std::wstring& Name)
{
    _AcceleratorNames.push_back(KeyNamePair(Code, Name));
}

void InitAccelerator()
{
    _AcceleratorNames.reserve(100);
    _AcceleratorNames.insert(_AcceleratorNames.end(),
    {
        {192, L"`"},
        {27, L"Esc"},
        {189, L"-"},
        {187, L"="},
        {220, L"\\"},
        {8, L"Backspace"},
        {219, L"["},
        {221, L"]"},
        {186, L","},
        {222, L"\'"},
        {188, L","},
        {190, L"."},
        {191, L"/"},
        {45, L"Ins"},
        {46, L"Del"},
        {36, L"Home"},
        {35, L"End"},
        {33, L"PageUp"},
        {34, L"PageDown"},
    });
    for (int i = 0; i < 26; i++)
    {
        AddAcceleratorName(65 + i, std::wstring(1, L'A' + i));
    }
    for (int i = 0; i < 10; i++)
    {
        AddAcceleratorName(48 + i, std::to_wstring(i));
    }
    for (int i = 0; i < 12; i++)
    {
        AddAcceleratorName(112 + i, L"F" + std::to_wstring(i + 1));
    }
    for (int i = 0; i < 10; i++)
    {
        AddAcceleratorName(96 + i, L"Num" + std::to_wstring(i));
    }

    _AcceleratorManager = new AcceleratorManager;
}

void DestroyAccelerator()
{
    delete _AcceleratorManager;
    _AcceleratorManager = nullptr;
}

int KeyCodeFromName(const std::wstring& Name)
{
    for (auto& Pair : _AcceleratorNames)
    {
        if (Pair.Name == Name)
        {
            return Pair.Code;
        }
    }
    return 0;
}

bool IAcceleratorReceiver::Bind(AcceleratorItem* Item)
{
    if (Item)
    {
        if (Item->Bind(this))
        {
            Unbind();
            _Item = Item;
            OnAttachAcceleratorItem();
            return true;
        }
    }
    return false;
}

void IAcceleratorReceiver::Unbind()
{
    if (_Item)
    {
        _Item->Unbind();
        _Item = nullptr;
        OnDetachAcceleratorItem();
    }
}

AcceleratorItem::AcceleratorItem(const std::wstring& Name)
    : _Name(Name)
{
}

AcceleratorItem::~AcceleratorItem()
{
    if (_Receiver)
    {
        _Receiver->Unbind();
    }
}

bool AcceleratorItem::Bind(IAcceleratorReceiver* Receiver)
{
    if (_Receiver)
    {
        return false;
    }
    else
    {
        _Receiver = Receiver;
        return true;
    }
}

AcceleratorManager::AcceleratorManager()
    : _AcceleratorList(1)
    , _Handle(CreateAcceleratorTableW(&_AcceleratorList[0], _AcceleratorList.size()))
{
}

AcceleratorManager::~AcceleratorManager()
{
    DestroyAcceleratorTable(_Handle);
}

AcceleratorItem* AcceleratorManager::GetAcceleratorItem(AcceleratorKey SysKeys, std::wstring& Key)
{
    ACCEL Info{FVIRTKEY, 0, 0};

    if (SysKeys & AcceleratorKey::Alt)
    {
        Info.fVirt |= FALT;
    }
    if (SysKeys & AcceleratorKey::Shift)
    {
        Info.fVirt |= FSHIFT;
    }
    if (SysKeys & AcceleratorKey::Ctrl)
    {
        Info.fVirt |= FCONTROL;
    }
    Info.key = KeyCodeFromName(Key);
    if (Info.key == 0)
    {
        return false;
    }

    for (size_t i = 0; i < _AcceleratorList.size(); i++)
    {
        ACCEL& Accel = _AcceleratorList[i];
        if (Accel.fVirt == Info.fVirt && Accel.key == Info.key)
        {
            return _AcceleratorItems[i].get();
        }
    }

    Info.cmd = static_cast<WORD>(_AcceleratorList.size());
    if (SysKeys & AcceleratorKey::Alt)
    {
        Key = L"Alt" + Key;
    }
    if (SysKeys & AcceleratorKey::Shift)
    {
        Key = L"Shift" + Key;
    }
    if (SysKeys & AcceleratorKey::Ctrl)
    {
        Key = L"Ctrl" + Key;
    }
    auto NewItem = std::make_shared<AcceleratorItem>(Key);
    _AcceleratorItems.push_back(NewItem);

    DestroyAcceleratorTable(_Handle);
    _AcceleratorList.push_back(Info);
    _Handle = CreateAcceleratorTableW(&_AcceleratorList[0], _AcceleratorList.size());
    return NewItem.get();
}

AcceleratorItem* AcceleratorManager::GetAcceleratorItem(int ID)
{
    if (ID >= 0 && ID < static_cast<int>(_AcceleratorItems.size()))
    {
        return _AcceleratorItems[ID].get();
    }
    else
    {
        return nullptr;
    }
}

/*Placement*/

Placement::Placement()
    : _Container(nullptr)
    , _Parent(nullptr)
    , _Control(nullptr)
    , _Placement1(nullptr)
    , _Placement2(nullptr)
    , _SpliterPosition(0)
    , _SpliterDirection(PlacementDirection::Horizontal)
    , _SpliterSize(0)
    , _SpliterScale(0.0)
    , _Behavior(PlacementBehavior::FixScale)
    , _BorderSize(0)
    , _MinClientWidth(0)
    , _MinClientHeight(0)
    , _DisableResize(false)
{
}

Placement::Placement(Placement* Parent)
    : Placement()
{
    _Parent = Parent;
}

Placement::Placement(WinContainer* Container)
    : Placement()
{
    _Container = Container;
    _Container->OnSizing.Bind(this, &Placement::OnSizing);
    _Container->OnSized.Bind(this, &Placement::OnSized);
}

Placement::~Placement()
{
    if (_Placement1)
    {
        delete _Placement1;
    }
    if (_Placement2)
    {
        delete _Placement2;
    }
    if (_Container)
    {
        _Container->OnSizing.Unbind(this, &Placement::OnSizing);
        _Container->OnSized.Unbind(this, &Placement::OnSized);
    }
}

void Placement::OnSizing(Object* Sender, bool Left, bool Right, bool Top, bool Bottom, LPRECT Area)
{
    if (_DisableResize)
    {
        return;
    }
    RECT Min = GetMinSize();
    int MinWidth = Min.right + (_Container->GetWidth() - _Container->GetClientWidth());
    int MinHeight = Min.bottom + (_Container->GetHeight() - _Container->GetClientHeight());
    int Width = Area->right - Area->left;
    int Height = Area->bottom - Area->top;
    if (Width < MinWidth)
    {
        if (Left)
        {
            Area->left = Area->right - MinWidth;
        }
        else
        {
            Area->right = Area->left + MinWidth;
        }
    }
    if (Height < MinHeight)
    {
        if (Top)
        {
            Area->top = Area->bottom - MinHeight;
        }
        else
        {
            Area->bottom = Area->top + MinHeight;
        }
    }
}

void Placement::OnSized(Object* Sender)
{
    if (_DisableResize)
    {
        return;
    }
    Resize();
}

void Placement::Resize()
{
    if (_Container)
    {
        _DisableResize = true;
        RECT MinSize = GetMinSize();
        if (_Container->GetClientWidth() < MinSize.right)
        {
            _Container->SetClientWidth(MinSize.right);
        }
        if (_Container->GetClientHeight() < MinSize.bottom)
        {
            _Container->SetClientHeight(MinSize.bottom);
        }
        _DisableResize = false;
    }

    RECT Area = GetArea();
    if (_Control)
    {
        _Control->SetLeft(Area.left);
        _Control->SetTop(Area.top);
        _Control->SetWidth(Area.right - Area.left);
        _Control->SetHeight(Area.bottom - Area.top);
    }
    if (_Behavior == PlacementBehavior::FixScale)
    {
        if (_SpliterDirection == PlacementDirection::Horizontal)
        {
            _SpliterPosition = static_cast<int>((Area.bottom - Area.top)*_SpliterScale);
        }
        else
        {
            _SpliterPosition = static_cast<int>((Area.right - Area.left)*_SpliterScale);
        }
    }
    if (_Placement1)
    {
        _Placement1->Resize();
    }
    if (_Placement2)
    {
        _Placement2->Resize();
    }
}

void Placement::UnregisterControl(WinControl* Control)
{
    if (_Control == Control)
    {
        _Control = nullptr;
    }
    if (_Placement1)
    {
        _Placement1->UnregisterControl(Control);
    }
    if (_Placement2)
    {
        _Placement2->UnregisterControl(Control);
    }
}

Placement* Placement::GetPlacement1()
{
    if (!_Placement1)
    {
        _Placement1 = new Placement(this);
    }
    return _Placement1;
}

Placement* Placement::GetPlacement2()
{
    if (!_Placement2)
    {
        _Placement2 = new Placement(this);
    }
    return _Placement2;
}

RECT Placement::GetArea()
{
    RECT Area;
    if (_Parent)
    {
        Area = _Parent->GetArea();
        if (_Parent->_Placement1 == this)
        {
            if (_Parent->_SpliterDirection == PlacementDirection::Horizontal)
            {
                Area.bottom = Area.top + _Parent->GetPlacement1Position();
            }
            else
            {
                Area.right = Area.left + _Parent->GetPlacement1Position();
            }
        }
        else
        {
            if (_Parent->_SpliterDirection == PlacementDirection::Horizontal)
            {
                Area.top = Area.bottom - _Parent->GetPlacement2Position();
            }
            else
            {
                Area.left = Area.right - _Parent->GetPlacement2Position();
            }
        }
    }
    else
    {
        Area.left = 0;
        Area.top = 0;
        Area.right = _Container->GetClientWidth();
        Area.bottom = _Container->GetClientHeight();
    }
    Area.left += _BorderSize;
    Area.top += _BorderSize;
    Area.right -= _BorderSize;
    Area.bottom -= _BorderSize;
    if (Area.right < Area.left)
    {
        Area.right = Area.left;
    }
    if (Area.bottom < Area.top)
    {
        Area.bottom = Area.top;
    }
    return Area;
}

RECT Placement::GetMinSize()
{
    RECT Min1 = {0}, Min2 = {0}, Min = {0};

    if (_Placement1)
    {
        Min1 = _Placement1->GetMinSize();
    }
    if (_Placement2)
    {
        Min2 = _Placement2->GetMinSize();
    }
    int MaxWidth = Max(Min1.right, Min2.right);
    int MaxHeight = Max(Min1.bottom, Min2.bottom);
    if (_SpliterDirection == PlacementDirection::Horizontal)
    {
        Min.right = MaxWidth + _BorderSize * 2;
        Min.bottom = Min1.bottom + Min2.bottom + _BorderSize * 2 + _SpliterSize;
    }
    else
    {
        Min.right = Min1.right + Min2.right + _BorderSize * 2 + _SpliterSize;
        Min.bottom = MaxHeight + _BorderSize * 2;
    }

    int MinWidth = _MinClientWidth + 2 * _BorderSize;
    int MinHeight = _MinClientHeight + 2 * _BorderSize;
    if (Min.right < MinWidth)
    {
        Min.right = MinWidth;
    }
    if (Min.bottom < MinHeight)
    {
        Min.bottom = MinHeight;
    }
    return Min;
}

int Placement::GetPlacement1Position()
{
    if (_Behavior == PlacementBehavior::FixPlacement2)
    {
        RECT Area = GetArea();
        if (_SpliterDirection == PlacementDirection::Horizontal)
        {
            return Area.bottom - Area.top - _SpliterSize - _SpliterPosition;
        }
        else
        {
            return Area.right - Area.left - _SpliterSize - _SpliterPosition;
        }
    }
    else
    {
        return _SpliterPosition;
    }
}

void Placement::SetPlacement1Position(int Value)
{
    if (_Behavior == PlacementBehavior::FixPlacement2)
    {
        RECT Area = GetArea();
        if (_SpliterDirection == PlacementDirection::Horizontal)
        {
            SetSpliterPosition(Area.bottom - Area.top - _SpliterSize - Value);
        }
        else
        {
            SetSpliterPosition(Area.right - Area.left - _SpliterSize - Value);
        }
    }
    else
    {
        SetSpliterPosition(Value);
    }
}

int Placement::GetPlacement2Position()
{
    if (_Behavior == PlacementBehavior::FixPlacement1)
    {
        RECT Area = GetArea();
        if (_SpliterDirection == PlacementDirection::Horizontal)
        {
            return Area.bottom - Area.top - _SpliterSize - _SpliterPosition;
        }
        else
        {
            return Area.right - Area.left - _SpliterSize - _SpliterPosition;
        }
    }
    else
    {
        return _SpliterPosition;
    }
}

void Placement::SetPlacement2Position(int Value)
{
    if (_Behavior == PlacementBehavior::FixPlacement1)
    {
        RECT Area = GetArea();
        if (_SpliterDirection == PlacementDirection::Horizontal)
        {
            SetSpliterPosition(Area.bottom - Area.top - _SpliterSize - Value);
        }
        else
        {
            SetSpliterPosition(Area.right - Area.left - _SpliterSize - Value);
        }
    }
    else
    {
        SetSpliterPosition(Value);
    }
}

namespace placement
{
class Control_: public Base
{
protected:
    WinControl* _Control;
    int _Border;
    int _MinWidth;
    int _MinHeight;
public:
    Control_(WinControl* Control, int Border, int MinWidth, int MinHeight)
        : _Control(Control)
        , _Border(Border)
        , _MinWidth(MinWidth)
        , _MinHeight(MinHeight)
    {
    }

    void Apply(Placement* Placement)
    {
        Placement->SetBorderSize(_Border);
        Placement->SetMinClientWidth(_MinWidth);
        Placement->SetMinClientHeight(_MinHeight);
        Placement->SetControl(_Control);
    }
};

class Part_: public Base
{
public:
    enum class Behavior
    {
        Fix1, Fix2, Scale
    };
protected:
    bool _Horizontal;
    int _SpliterSize;
    double _SpliterScale;
    int _Border;
    Behavior _Behavior;
    Base::Pointer _Placement1;
    Base::Pointer _Placement2;
public:
    Part_(bool Horizontal, int SpliterSize, double SpliterScale, int Border, Behavior PlacementBehavior, Base::Pointer Placement1, Base::Pointer Placement2)
        : _Horizontal(Horizontal)
        , _SpliterSize(SpliterSize)
        , _SpliterScale(SpliterScale)
        , _Border(Border)
        , _Behavior(PlacementBehavior)
        , _Placement1(Placement1)
        , _Placement2(Placement2)
    {
    }

    void Apply(Placement* Placement)
    {
        Placement->SetSpliterDirection(_Horizontal ? PlacementDirection::Horizontal : PlacementDirection::Vertical);
        Placement->SetSpliterSize(_SpliterSize);
        Placement->SetBorderSize(_Border);
        switch (_Behavior)
        {
        case Behavior::Fix1:
            Placement->SetBehavior(PlacementBehavior::FixPlacement1);
            break;
        case Behavior::Fix2:
            Placement->SetBehavior(PlacementBehavior::FixPlacement2);
            break;
        case Behavior::Scale:
            Placement->SetBehavior(PlacementBehavior::FixScale);
            Placement->SetSpliterScale(_SpliterScale);
            break;
        }
        _Placement1->Apply(Placement->GetPlacement1());
        _Placement2->Apply(Placement->GetPlacement2());
        RECT MinSize1 = Placement->GetPlacement1()->GetMinSize();
        RECT MinSize2 = Placement->GetPlacement2()->GetMinSize();
        int Min1 = _Horizontal ? (MinSize1.bottom - MinSize1.top) : (MinSize1.right - MinSize1.left);
        int Min2 = _Horizontal ? (MinSize2.bottom - MinSize2.top) : (MinSize2.right - MinSize2.left);
        switch (_Behavior)
        {
        case Behavior::Fix1:
            Placement->SetPlacement1Position(Min1);
            break;
        case Behavior::Fix2:
            Placement->SetPlacement2Position(Min2);
            break;
        case Behavior::Scale:
            if (Min1)
            {
                Placement->SetPlacement1Position(Min1);
            }
            else
            {
                Placement->SetPlacement2Position(Min2);
            }
            break;
        }
    }
};

Blank::Blank(int MinWidth, int MinHeight)
    : _MinWidth(MinWidth)
    , _MinHeight(MinHeight)
{
}

Blank::operator Base::Pointer()
{
    return std::make_shared<Control_>(nullptr, 0, _MinWidth, _MinHeight);
}

Control::Control(WinControl* Control)
    : Control(Control, 0, 0, 0)
{
}

Control::Control(WinControl* Control, int Border)
    : Control(Control, Border, 0, 0)
{
}

Control::Control(WinControl* Control, int MinWidth, int MinHeight)
    : Control(Control, 0, MinWidth, MinHeight)
{
}

Control::Control(WinControl* Control, int Border, int MinWidth, int MinHeight)
    : _Control(Control)
    , _Border(Border)
    , _MinWidth(MinWidth)
    , _MinHeight(MinHeight)
{
}

Control::operator Base::Pointer()
{
    return std::make_shared<Control_>(_Control, _Border, _MinWidth, _MinHeight);
}

VertFix1::VertFix1(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2)
    : _Border(Border)
    , _SpliterSize(SpliterSize)
    , _Placement1(Placement1)
    , _Placement2(Placement2)
{
}

VertFix1::operator Base::Pointer()
{
    return std::make_shared<Part_>(false, _SpliterSize, 0, _Border, Part_::Behavior::Fix1, _Placement1, _Placement2);
}

VertFix2::VertFix2(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2)
    : _Border(Border)
    , _SpliterSize(SpliterSize)
    , _Placement1(Placement1)
    , _Placement2(Placement2)
{
}

VertFix2::operator Base::Pointer()
{
    return std::make_shared<Part_>(false, _SpliterSize, 0, _Border, Part_::Behavior::Fix2, _Placement1, _Placement2);
}

VertScale::VertScale(int Border, int SpliterSize, double Scale, Base::Pointer Placement1, Base::Pointer Placement2)
    : _Border(Border)
    , _SpliterSize(SpliterSize)
    , _Scale(Scale)
    , _Placement1(Placement1)
    , _Placement2(Placement2)
{
}

VertScale::operator Base::Pointer()
{
    return std::make_shared<Part_>(false, _SpliterSize, _Scale, _Border, Part_::Behavior::Scale, _Placement1, _Placement2);
}

HorzFix1::HorzFix1(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2)
    : _Border(Border)
    , _SpliterSize(SpliterSize)
    , _Placement1(Placement1)
    , _Placement2(Placement2)
{
}

HorzFix1::operator Base::Pointer()
{
    return std::make_shared<Part_>(true, _SpliterSize, 0, _Border, Part_::Behavior::Fix1, _Placement1, _Placement2);
}

HorzFix2::HorzFix2(int Border, int SpliterSize, Base::Pointer Placement1, Base::Pointer Placement2)
    : _Border(Border)
    , _SpliterSize(SpliterSize)
    , _Placement1(Placement1)
    , _Placement2(Placement2)
{
}

HorzFix2::operator Base::Pointer()
{
    return std::make_shared<Part_>(true, _SpliterSize, 0, _Border, Part_::Behavior::Fix2, _Placement1, _Placement2);
}

HorzScale::HorzScale(int Border, int SpliterSize, double Scale, Base::Pointer Placement1, Base::Pointer Placement2)
    : _Border(Border)
    , _SpliterSize(SpliterSize)
    , _Scale(Scale)
    , _Placement1(Placement1)
    , _Placement2(Placement2)
{
}

HorzScale::operator Base::Pointer()
{
    return std::make_shared<Part_>(true, _SpliterSize, _Scale, _Border, Part_::Behavior::Scale, _Placement1, _Placement2);
}

} //placement


WinClass::WinClass(const std::wstring& Name, bool Shadow, bool OwnDC, WNDPROC Procedure, HINSTANCE hInstance)
    : _Name(Name)
    , _WindowClass{sizeof(_WindowClass)}
    , _ClassAtom(0)
{
    _WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | (Shadow ? CS_DROPSHADOW : 0) | (OwnDC ? CS_OWNDC : 0);
    _WindowClass.lpfnWndProc = Procedure;
    _WindowClass.cbClsExtra = 0;
    _WindowClass.cbWndExtra = 0;
    _WindowClass.hInstance = hInstance;
    _WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    _WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    _WindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    _WindowClass.lpszMenuName = NULL;
    _WindowClass.lpszClassName = _Name.c_str();
    _WindowClass.hIconSm = NULL;

    _ClassAtom = RegisterClassExW(&_WindowClass);
}

bool WinControl::_CreateWindow(DWORD ExStyle, DWORD Style, const wchar_t * ClassName, WinContainer* Parent)
{
    if (_Handle == NULL)
    {
        _Handle = CreateWindowExW(
            ExStyle,
            ClassName,
            L"",
            Style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            Parent ? Parent->GetHandle() : NULL,
            NULL,
            GetApplication()->GetInstance(),
            NULL);
        if (_Handle != NULL)
        {
            GetApplication()->_Controls.insert({_Handle, this});
            if (ClassName != GetDefaultClass()->GetName())
            {
                SetWindowSubclass(_Handle, SubclassProc, 0, 0);
            }
            if (Parent)
            {
                SetParent(Parent);
            }
            SetFont(WinFont::FromWindow(_Handle, L"微软雅黑", 10));
            return true;
        }
        else
        {
            DWORD Error = GetLastError();
            assert(0);
        }
    }
    return false;
}


DWORD WinControl::InternalGetExStyle()
{
    return GetWindowLongW(_Handle, GWL_EXSTYLE);
}

void WinControl::InternalSetExStyle(DWORD ExStyle)
{
    SetWindowLongW(_Handle, GWL_EXSTYLE, ExStyle);
    SetWindowPos(_Handle, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool WinControl::GetExStyle(DWORD ExStyle)
{
    return (InternalGetExStyle() & ExStyle) != 0;
}

void WinControl::SetExStyle(DWORD ExStyle, bool Available)
{
    LONG_PTR Long = InternalGetExStyle();
    if (Available)
    {
        Long |= ExStyle;
    }
    else
    {
        Long &= ~ExStyle;
    }
    InternalSetExStyle(Long);
}

bool WinControl::GetStyle(DWORD Style)
{
    return (GetWindowLongW(_Handle, GWL_STYLE) & Style) != 0;
}

void WinControl::SetStyle(DWORD Style, bool Available)
{
    LONG_PTR Long = GetWindowLongW(_Handle, GWL_STYLE);
    if (Available)
    {
        Long |= Style;
    }
    else
    {
        Long &= ~Style;
    }
    SetWindowLongW(_Handle, GWL_STYLE, Long);
    SetWindowPos(_Handle, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void WinControl::TrackMouse(bool Enable, int HoverTime)
{
    TRACKMOUSEEVENT Event{sizeof(Event)};
    Event.dwFlags = (Enable ? 0 : TME_CANCEL) | TME_HOVER | TME_LEAVE;
    Event.hwndTrack = _Handle;
    Event.dwHoverTime = HoverTime == -1 ? HOVER_DEFAULT : HoverTime;
    TrackMouseEvent(&Event);
}

void WinControl::Destroy()
{
    if (_Parent)
    {
        _Parent->UnregisterChild(this);
        _Parent = nullptr;
    }
    else if (_Handle)
    {
        if (_SubClassed)
        {
            RemoveWindowSubclass(_Handle, SubclassProc, 0);
        }
        DestroyWindow(_Handle);
    }
    if (_Handle)
    {
        GetApplication()->_Controls.erase(_Handle);
        _Handle = 0;
    }
}

void WinControl::GetWindowPosSize(int& Left, int& Top, int& Width, int& Height)
{
    RECT Rect;
    GetWindowRect(_Handle, &Rect);
    if (_Parent)
    {
        POINT Point;
        Point.x = Rect.left;
        Point.y = Rect.top;
        MapWindowPoints(NULL, _Parent->_Handle, &Point, 1);
        Left = Point.x;
        Top = Point.y;
    }
    else
    {
        Left = Rect.left;
        Top = Rect.top;
    }
    Width = Rect.right - Rect.left;
    Height = Rect.bottom - Rect.top;
}

WinControl::WinControl()
    : _Handle(0)
    , _MouseEntered(false)
    , _HoverOnce(true)
    , _EnableHover(false)
    , _HoverTime(-1)
    , _LastX(-1)
    , _LastY(-1)
    , _Parent(nullptr)
    , _SubClassed(false)
{
}

WinControl::WinControl(DWORD exStyle, DWORD style, const wchar_t* className, WinContainer* parent)
    : _Handle(CreateWindowExW(
        exStyle, className, L"", style,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        parent ? parent->GetHandle() : NULL,
        NULL,
        GetApplication()->GetInstance(),
        NULL))
    , _MouseEntered(false)
    , _HoverOnce(true)
    , _EnableHover(false)
    , _HoverTime(-1)
    , _LastX(-1)
    , _LastY(-1)
    , _Parent(parent)
    , _Font(WinFont::FromWindow(_Handle, L"微软雅黑", 10))
    , _SubClassed(false)
{
    if (_Handle != NULL)
    {
        GetApplication()->_Controls.insert({_Handle, this});
        if (className != GetDefaultClass()->GetName())
        {
            SetWindowSubclass(_Handle, SubclassProc, 0, 0);
        }
        if (parent)
        {
            if (::SetParent(_Handle, parent->GetHandle()) != NULL)
            {
                _Parent->RegisterChild(this);
                SetWindowPos(_Parent->GetHandle(), NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
        }
        //set font
        SendMessageW(_Handle, WM_SETFONT, reinterpret_cast<WPARAM>(_Font->GetHandle()), TRUE);
    }
    else
    {
        DWORD Error = GetLastError();
        assert(0);
    }
}

WinControl::~WinControl()
{
    Destroy();
}

LRESULT WinControl::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
    LRESULT Result = 0;
    switch (Message)
    {
    case WM_GETPOINTER_DISPATCHED:
        Result = reinterpret_cast<LRESULT>(this);
        CallDefaultProc = false;
        break;
    case WM_ENABLE:
        if (wParam == TRUE)
        {
            OnEnabled(this);
        }
        else
        {
            OnDisabled(this);
        }
        break;
    case WM_MOVE:
        OnMoved(this);
        break;
    case WM_MOVING:
        OnMoving(this, reinterpret_cast<LPRECT>(lParam));
        Result = TRUE;
        break;
    case WM_SHOWWINDOW:
        if (wParam == TRUE)
        {
            OnShow(this);
        }
        else
        {
            OnHide(this);
        }
        break;
    case WM_SIZE:
        OnSized(this);
        break;
    case WM_SIZING:
    {
        bool Left = false, Right = false, Top = false, Bottom = false;
        switch (wParam)
        {
        case WMSZ_BOTTOM:
            Bottom = true;
            break;
        case WMSZ_BOTTOMLEFT:
            Left = true;
            Bottom = true;
            break;
        case WMSZ_BOTTOMRIGHT:
            Right = true;
            Bottom = true;
            break;
        case WMSZ_LEFT:
            Left = true;
            break;
        case WMSZ_RIGHT:
            Right = true;
            break;
        case WMSZ_TOP:
            Top = true;
            break;
        case WMSZ_TOPLEFT:
            Left = true;
            Top = true;
            break;
        case WMSZ_TOPRIGHT:
            Right = true;
            Top = true;
            break;
        }
        OnSizing(this, Left, Right, Top, Bottom, (LPRECT)lParam);
        Result = TRUE;
        break;
    }
    case WM_SETFOCUS:
        OnGotFocus(this);
        break;
    case WM_KILLFOCUS:
        OnLostFocus(this);
        break;
    case WM_PAINT:
        OnPaint(this);
        break;
    case WM_MOUSEMOVE:
    {
        MouseStruct MouseStruct(wParam, lParam, false);
        if (_LastX != MouseStruct.X || _LastY != MouseStruct.Y)
        {
            _LastX = MouseStruct.X;
            _LastY = MouseStruct.Y;
            OnMouseMove(this, MouseStruct);
            if (!_MouseEntered)
            {
                _MouseEntered = true;
                OnMouseEnter(this);
                TrackMouse(true, _HoverTime);
            }
            else if (_HoverOnce)
            {
                TrackMouse(true, _HoverTime);
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
        OnLeftButtonDown(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_LBUTTONUP:
        OnLeftButtonUp(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_LBUTTONDBLCLK:
        OnLeftButtonDblClick(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_RBUTTONDOWN:
        OnRightButtonDown(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_RBUTTONUP:
        OnRightButtonUp(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_RBUTTONDBLCLK:
        OnRightButtonDblClick(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_MBUTTONDOWN:
        OnMiddleButtonDown(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_MBUTTONUP:
        OnMiddleButtonUp(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_MBUTTONDBLCLK:
        OnMiddleButtonDblClick(this, MouseStruct(wParam, lParam, false));
        break;
    case WM_MOUSEHWHEEL:
        OnMouseHWheel(this, MouseStruct(wParam, lParam, true));
        break;
    case WM_MOUSEWHEEL:
        OnMouseVWheel(this, MouseStruct(wParam, lParam, true));
        break;
    case WM_MOUSEHOVER:
        if (_EnableHover)
        {
            OnMouseHover(this, MouseStruct(wParam, lParam, false));
            if (!_HoverOnce)
            {
                TrackMouse(true, _HoverTime);
            }
        }
        break;
    case WM_MOUSELEAVE:
        OnMouseLeave(this);
        _MouseEntered = false;
        break;
    case WM_KEYDOWN:
        OnKeyDown(this, KeyStruct(wParam, lParam));
        break;
    case WM_KEYUP:
        OnKeyUp(this, KeyStruct(wParam, lParam));
        break;
    case WM_SYSKEYDOWN:
        OnSysKeyDown(this, KeyStruct(wParam, lParam));
        break;
    case WM_SYSKEYUP:
        OnSysKeyUp(this, KeyStruct(wParam, lParam));
        break;
    case WM_CHAR:
    {
        wchar_t Char = wParam;
        OnChar(this, Char);
        wParam = Char;
        if (wParam == 0)
        {
            CallDefaultProc = false;
        }
        break;
    }
    case WM_COMMAND:
        if (lParam != 0)
        {
            WinControl* Control = GetApplication()->GetControl(reinterpret_cast<HWND>(lParam));
            if (Control)
            {
                Result = Control->ProcessMessage(WM_COMMAND_DISPATCHED, wParam, lParam, CallDefaultProc);
            }
        }
        break;
    case WM_NOTIFY:
    {
        NMHDR* hdr = reinterpret_cast<NMHDR*>(lParam);
        WinControl* Control = GetApplication()->GetControl(hdr->hwndFrom);
        if (Control)
        {
            Result = Control->ProcessMessage(WM_NOTIFY_DISPATCHED, wParam, lParam, CallDefaultProc);
        }
        break;
    }
    case WM_VSCROLL:
    {
        WinControl* Control = GetApplication()->GetControl(reinterpret_cast<HWND>(lParam));
        if (Control)
        {
            Result = Control->ProcessMessage(WM_VSCROLL_DISPATCHED, wParam, lParam, CallDefaultProc);
        }
        break;
    }
    case WM_HSCROLL:
    {
        WinControl* Control = GetApplication()->GetControl(reinterpret_cast<HWND>(lParam));
        if (Control)
        {
            Result = Control->ProcessMessage(WM_HSCROLL_DISPATCHED, wParam, lParam, CallDefaultProc);
        }
        break;
    }
    } //switch
    return Result;
}

void WinControl::SetParent(WinContainer* Parent)
{
    HWND NewParent = Parent ? Parent->GetHandle() : NULL;
    if (::SetParent(_Handle, NewParent) != NULL)
    {
        if (_Parent)
        {
            _Parent->UnregisterChild(this);
            SetWindowPos(_Parent->GetHandle(), NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
        _Parent = Parent;
        if (_Parent)
        {
            _Parent->RegisterChild(this);
            SetWindowPos(_Parent->GetHandle(), NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
    }
}

int WinControl::GetLeft()
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    return l;
}

void WinControl::SetLeft(int Value)
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    l = Value;
    SetWindowPosSize(l, t, w, h);
}

int WinControl::GetTop()
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    return t;
}

void WinControl::SetTop(int Value)
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    t = Value;
    SetWindowPosSize(l, t, w, h);
}

int WinControl::GetWidth()
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    return w;
}

void WinControl::SetWidth(int Value)
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    w = Value;
    SetWindowPosSize(l, t, w, h);
}

int WinControl::GetHeight()
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    return h;
}

void WinControl::SetHeight(int Value)
{
    int l, t, w, h;
    GetWindowPosSize(l, t, w, h);
    h = Value;
    SetWindowPosSize(l, t, w, h);
}

void WinControl::Move(int Left, int Top, int Width, int Height)
{
    MoveWindow(_Handle, Left, Top, Width, Height, TRUE);
}

std::wstring WinControl::GetText()
{
    int Length = GetWindowTextLengthW(_Handle);
    std::wstring Text(Length + 1, '\0');
    GetWindowTextW(_Handle, &Text[0], Length + 1);
    return Text;
}

void WinControl::SetText(const std::wstring& Value)
{
    SetWindowTextW(_Handle, Value.c_str());
}

void WinContainer::RegisterChild(WinControl* Control)
{
    if (std::find(_Controls.begin(), _Controls.end(), Control) == _Controls.end())
    {
        _Controls.push_back(Control);
    }
}

void WinContainer::UnregisterChild(WinControl* Control)
{
    if (_Placement)
    {
        _Placement->UnregisterControl(Control);
    }
    auto Place = std::find(_Controls.begin(), _Controls.end(), Control);
    if (Place != _Controls.end())
    {
        _Controls.erase(Place);
    }
}

void WinContainer::Destroy()
{
    if (_Placement)
    {
        delete _Placement;
        _Placement = nullptr;
    }
    while (_Controls.size())
    {
        delete *(_Controls.end() - 1);
    }
//    WinControl::Destroy();
}

WinContainer::WinContainer()
    : WinControl()
    , _Controls()
    , _Placement(nullptr)
{
}

pl::windows::WinContainer::WinContainer(DWORD exStyle, DWORD style, const wchar_t * className, WinContainer * parent)
    : WinControl(exStyle, style, className, parent)
    , _Controls()
    , _Placement(nullptr)
{
}

WinContainer::~WinContainer()
{
    Destroy();
}

int WinForm::RegisterOwned(WinTimer* Timer)
{
    static int TimerHandle = 1;
    if (std::find(_TimerList.begin(), _TimerList.end(), Timer) != _TimerList.end())
    {
        _TimerList.push_back(Timer);
    }
    return TimerHandle++;
}

void WinForm::UnregisterOwned(WinTimer* Timer)
{
    auto Place = std::find(_TimerList.begin(), _TimerList.end(), Timer);
    if (Place != _TimerList.end())
    {
        _TimerList.erase(Place);
    }
}

void WinForm::RegisterOwned(WinMenu* Menu)
{
    if (std::find(_MenuList.begin(), _MenuList.end(), Menu) != _MenuList.end())
    {
        _MenuList.push_back(Menu);
    }
}

void WinForm::UnregisterOwned(WinMenu* Menu)
{
    auto Place = std::find(_MenuList.begin(), _MenuList.end(), Menu);
    if (Place != _MenuList.end())
    {
        _MenuList.erase(Place);
    }
}

int WinForm::GenerateMenuItemHandle()
{
    if (_FreeMenuItemHandles.size())
    {
        int Return = _FreeMenuItemHandles.back();
        _FreeMenuItemHandles.pop_back();
        return Return;
    }
    else
    {
        return _UsedMenuItemHandle++;
    }
}

void WinForm::CreateForm()
{
    if (!_Created)
    {
        if (_CreateWindow(
            WS_EX_APPWINDOW | WS_EX_CONTROLPARENT,
            WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
            GetDefaultClass()->GetName().c_str(),
            nullptr))
        {
            _IsMainForm = GetApplication()->RegisterForm(this);
            InitializeComponents();
            OnCreate(this);
        }
        else
        {
            _IsMainForm = false;
        }
        _Created = true;
    }
}

WinForm::WinForm(bool Create)
    : WinContainer()
    , _IsMainForm(false)
    , _Created(false)
    , _HotKeys()
    , _TimerList()
    , _MenuList()
    , _FormMenu(nullptr)
    , _MenuItemMap()
    , _UsedMenuItemHandle(1)
    , _FreeMenuItemHandles()
{
    if (Create)
    {
        CreateForm();
    }
}

WinForm::WinForm()
     : WinContainer(WS_EX_APPWINDOW | WS_EX_CONTROLPARENT,
                   WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
                   GetDefaultClass()->GetName().c_str(),
                   nullptr)
    , _IsMainForm(GetApplication()->RegisterForm(this))
    , _Created(true)
    , _HotKeys()
    , _TimerList()
    , _MenuList()
    , _FormMenu(nullptr)
    , _MenuItemMap()
    , _UsedMenuItemHandle(1)
    , _FreeMenuItemHandles()
{
    InitializeComponents();
    OnCreate(this);
}

WinForm::~WinForm()
{
    GetApplication()->UnregisterForm(this);
    DestroyForm();
}

LRESULT WinForm::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
    LRESULT Result = 0;
    switch (Message)
    {
    case WM_ACTIVATEAPP:
        if (wParam == TRUE)	/*窗口被激活*/
        {
            OnActivateApp(this);
        }
        else				/*窗口被取消激活*/
        {
            OnDeactivateApp(this);
        }
    case WM_ACTIVATE:
        if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)	/*窗口被激活*/
        {
            OnActivate(this);
        }
        else				/*窗口被取消激活*/
        {
            OnDeactivate(this);
        }
        break;
    case WM_CLOSE:			/*需要关闭窗口*/
    {
        bool Cancel = false;
        OnQueryClose(this, Cancel);
        if (!Cancel)
        {
            OnClose(this);
            if (_IsMainForm)
            {
                GetApplication()->Terminate();
            }
        }
        else
        {
            CallDefaultProc = false;
        }
        break;
    }
    case WM_COMPACTING:		/*内存太少*/
        OnCompacting(this);
        break;
    case WM_CREATE:			/*创建窗口*/
        break;
    case WM_DESTROY:		/*释放窗口*/
        OnDestroy(this);

        /*删除Timer*/
        _TimerList.clear();
        /*删除菜单*/
        _MenuList.clear();
        /*注销热键*/
        for (int HotKey : _HotKeys)
        {
            UnregisterGlobalHotKey(HotKey);
        }
        _HotKeys.clear();

        if (_IsMainForm)
        {
            PostQuitMessage(0);
        }
        break;
    case WM_ENTERSIZEMOVE:	/*进入修改尺寸状态*/
        OnEnterSizeMove(this);
        break;
    case WM_EXITSIZEMOVE:	/*退出修改尺寸状态*/
        OnLeaveSizeMove(this);
        break;
    case WM_NCACTIVATE:
        if (wParam == TRUE)	/*非客户区被激活*/
            ;
        else				/*非客户区被取消激活*/
            ;
        Result = wParam;
        break;
    case WM_HOTKEY:			/*热键*/
        OnHotKey(this, wParam);
        break;
    case WM_TIMER:			/*定时器*/
        for (auto& Timer : _TimerList)
        {
            if (Timer->GetEnabled() && Timer->GetHandle() == wParam)
            {
                Timer->OnTimer(Timer.Get());
            }
        }
        break;
    case WM_COMMAND:
        if (lParam != 0)
        {
            Result = WinContainer::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
        }
        else
        {
            switch (HIWORD(wParam))
            {
            case 0:				/*菜单*/
            {
                WinMenuItem* Item = MenuItemO_Handle(LOWORD(wParam));
                if (Item)
                {
                    Item->OnClick(Item);
                }
            }
            break;
            case 1:				/*加速键*/
            {
                AcceleratorItem* Item = GetAcceleratorManager()->GetAcceleratorItem(LOWORD(wParam));
                if (Item)
                {
                    Item->Execute();
                }
            }
            break;
            default:
                Result = WinContainer::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
            }
        }
        break;
    case WM_MENURBUTTONUP:	/*菜单右键消息*/
    {
        int Handle = GetMenuItemID((HMENU)lParam, wParam);
        WinMenuItem* Item = MenuItemO_Handle(Handle);
        if (Item)
        {
            Item->OnRightClick(Item);
        }
        break;
    }
    default:
        Result = WinContainer::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
    }
    return Result;
}

bool WinForm::RegisterGlobalHotKey(int ID, bool Alt, bool Ctrl, bool Shift, bool Win, int KeyCode)
{
    int Modifiers = 0;
    Alt ? Modifiers |= MOD_ALT : 1;
    Ctrl ? Modifiers |= MOD_CONTROL : 1;
    Shift ? Modifiers |= MOD_SHIFT : 1;
    Win ? Modifiers |= MOD_WIN : 1;
    if (RegisterHotKey(_Handle, ID, Modifiers, KeyCode) != 0)
    {
        _HotKeys.push_back(ID);
        return true;
    }
    else
    {
        return false;
    }
}

void WinForm::UnregisterGlobalHotKey(int ID)
{
    auto Place = std::find(_HotKeys.begin(), _HotKeys.end(), ID);
    if (Place != _HotKeys.end())
    {
        UnregisterHotKey(_Handle, *Place);
        _HotKeys.erase(Place);
    }
}

WinMenuItem* WinForm::MenuItemO_Handle(int Handle)
{
    auto Place = _MenuItemMap.find(Handle);
    if (Place == _MenuItemMap.end())
    {
        return 0;
    }
    else
    {
        return Place->second;
    }
}

WinFormBorder WinForm::GetBorder()
{
    if (GetStyle(WS_SIZEBOX))
    {
        return WinFormBorder::Sizable;
    }
    else if (GetStyle(WS_CAPTION))
    {
        return WinFormBorder::Single;
    }
    else if (GetStyle(WS_BORDER))
    {
        return WinFormBorder::Border;
    }
    else
    {
        return WinFormBorder::None;
    }
}

void WinForm::SetBorder(WinFormBorder Value)
{
    switch (Value)
    {
    case WinFormBorder::None:
        SetStyle(WS_BORDER, false);
        SetStyle(WS_CAPTION, false);
        SetStyle(WS_SIZEBOX, false);
        break;
    case WinFormBorder::Border:
        SetStyle(WS_BORDER, true);
        SetStyle(WS_CAPTION, false);
        SetStyle(WS_SIZEBOX, false);
        break;
    case WinFormBorder::Single:
        SetStyle(WS_BORDER, true);
        SetStyle(WS_CAPTION, true);
        SetStyle(WS_SIZEBOX, false);
        break;
    case WinFormBorder::Sizable:
        SetStyle(WS_BORDER, true);
        SetStyle(WS_CAPTION, true);
        SetStyle(WS_SIZEBOX, true);
        break;
    }
}

void WinForm::ShowModal()
{
    ShowWindow(_Handle, SW_SHOWNORMAL);
    GetApplication()->RunModal(this);
}

void WinForm::MoveCenter()
{
    RECT Area, Size;
    GetWindowRect(_Handle, &Size);
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &Area, 0);
    int Width = Size.right - Size.left;
    int Height = Size.bottom - Size.top;
    int ScreenWidth = Area.right - Area.left;
    int ScreenHeight = Area.bottom - Area.top;
    MoveWindow(_Handle, Area.left + (ScreenWidth - Width) / 2, Area.top + (ScreenHeight - Height) / 2, Width, Height, GetVisible() ? TRUE : FALSE);
}

void WinForm::SetFormMenu(WinFormMenu* Value)
{
    if (_FormMenu)
    {
        _FormMenu->_Parent = nullptr;
        _FormMenu->_Associated = false;
    }
    _FormMenu = Value;
    if (_FormMenu)
    {
        _FormMenu->_Parent = this;
        _FormMenu->_Associated = true;
        SetMenu(_Handle, _FormMenu->GetHandle());
    }
    else
    {
        SetMenu(_Handle, NULL);
    }
}

/*WinTimer*/

WinTimer::WinTimer(WinForm* Owner)
    : _Owner(Owner)
    , _Handle(Owner->RegisterOwned(this))
    , _Interval(1000)
    , _Enabled(false)
{
}

WinTimer::~WinTimer()
{
    _Owner->UnregisterOwned(this);
    if (_Enabled)
    {
        KillTimer(_Owner->GetHandle(), _Handle);
    }
}

void WinTimer::SetInterval(int Value)
{
    _Interval = Value;
    if (_Enabled)
    {
        SetTimer(_Owner->GetHandle(), _Handle, _Interval, NULL);
    }
}

void WinTimer::SetEnabled(bool Value)
{
    if (_Enabled != Value)
    {
        _Enabled = Value;
        if (_Enabled)
        {
            SetTimer(_Owner->GetHandle(), _Handle, _Interval, NULL);
        }
        else
        {
            KillTimer(_Owner->GetHandle(), _Handle);
        }
    }
}

/*WinImageList*/

WinBitmap::Pointer WinImageList::CheckBitmap(WinBitmap::Pointer Bitmap)
{
    if (Bitmap)
    {
        WinBitmap::Pointer DIB = 0;
        int w = GetImageWidth();
        int h = GetImageHeight();
        if (Bitmap->GetWidth() == w && Bitmap->GetHeight() == h && Bitmap->GetBitmapBits() == WinBitmap::BitmapBits::Bit32)
        {
            DIB = Bitmap;
        }
        else
        {
            DIB = std::make_shared<WinBitmap>(w, h, WinBitmap::BitmapBits::Bit32, true);
            DIB->GetWinDC()->Draw(0, 0, w, h, Bitmap);
            DIB->GenerateTrans(GetPixel(DIB->GetWinDC()->GetHandle(), 0, 0));
        }
        return DIB;
    }
    return 0;
}

WinImageList::WinImageList(int Width, int Height)
{
    _Handle = ImageList_Create(Width, Height, ILC_COLOR24 | ILC_MASK, 8, 8);
}

WinImageList::WinImageList(WinImageList* ImageList)
{
    _Handle = ImageList_Duplicate(ImageList->_Handle);
}

WinImageList::~WinImageList()
{
    ImageList_Destroy(_Handle);
}

void WinImageList::Add(WinBitmap::Pointer Bitmap)
{
    WinBitmap::Pointer Dest = CheckBitmap(Bitmap);
    if (GetCount() == 0)
    {
        SetBackgroundColor(GetPixel(Dest->GetWinDC()->GetHandle(), 0, 0));
    }
    //ModifyBitmap();
    int Index = ImageList_Add(_Handle, Dest->GetBitmap(), NULL);
}

void WinImageList::Replace(int Index, WinBitmap::Pointer Bitmap)
{
    WinBitmap::Pointer Dest = CheckBitmap(Bitmap);
    //ModifyBitmap();
    ImageList_Replace(_Handle, Index, Dest->GetBitmap(), NULL);
}

/*WinMenu*/

void WinMenu::ItemChanged()
{
}

WinMenu::WinMenu(WinForm* Owner)
    : _Handle(0)
    , _Owner(Owner)
    , _MessageDispatcher(Owner)
    , _Associated(false)
    , _MenuItems()
    , _AssociatedMenuItem(nullptr)
{
    if (Owner)
    {
        _Owner->RegisterOwned(this);
    }
}

WinMenu::~WinMenu()
{
    if (_Owner)
    {
        _Owner->UnregisterOwned(this);
    }
    for (WinMenuItem* Item : _MenuItems)
    {
        delete Item;
    }
    if (!_Associated)
    {
        DestroyMenu(_Handle);
    }
}

WinMenuItem* WinMenu::Insert(int Index)
{
    WinMenuItem* MenuItem = new WinMenuItem(this, _MessageDispatcher->GenerateMenuItemHandle());
    _MenuItems.insert(_MenuItems.begin() + Index, MenuItem);

    MENUITEMINFO Info;
    MenuItem->FillStruct(&Info);
    InsertMenuItemW(_Handle, Index, TRUE, &Info);

    ItemChanged();
    return MenuItem;
}

void WinMenu::Delete(int Index)
{
    delete _MenuItems[Index];
    _MenuItems.erase(_MenuItems.begin() + Index);
    ItemChanged();
}

void WinMenu::Delete(WinMenuItem* MenuItem)
{
    auto Place = std::find(_MenuItems.begin(), _MenuItems.end(), MenuItem);
    if (Place != _MenuItems.end())
    {
        _MenuItems.erase(Place);
    }
}

/*MenuItems*/

void WinMenuItem::OnAttachAcceleratorItem()
{
    RefreshProperties();
}

void WinMenuItem::OnDetachAcceleratorItem()
{
    RefreshProperties();
}

void WinMenuItem::OnExecuteAcceleratorItem()
{
    OnClick(this);
}

void WinMenuItem::FillStruct(MENUITEMINFO* Info)
{
    _MenuText = GetAcceleratorItem()
        ? _Text + L"\t" + GetAcceleratorItem()->GetName()
        : _Text;
    Info->cbSize = sizeof(*Info);
    Info->fMask = MIIM_CHECKMARKS | MIIM_BITMAP | MIIM_DATA | MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
    Info->fType = 0;
    Info->fType |= _Separator
        ? MFT_SEPARATOR
        : MFT_STRING;
    Info->fType |= _RadioCheck
        ? MFT_RADIOCHECK
        : 0;
    Info->fState = 0;
    Info->fState |= _Checked
        ? MFS_CHECKED
        : MFS_UNCHECKED;
    Info->fState |= _Enabled
        ? MFS_ENABLED
        : MFS_DISABLED;
    Info->fState |= _Highlighted
        ? MFS_HILITE
        : MFS_UNHILITE;
    Info->wID = _Handle;
    Info->hSubMenu = _SubMenu
        ? _SubMenu->GetHandle()
        : Info->hSubMenu = NULL;
    Info->hbmpChecked = NULL;
    Info->hbmpUnchecked = NULL;
    Info->dwItemData = reinterpret_cast<DWORD>(this);
    Info->dwTypeData = &_MenuText[0];
    Info->cch = _Text.length();
    Info->hbmpItem = _CheckedBitmap
        ? _CheckedBitmap->GetBitmap()
        : 0;
}

void WinMenuItem::RefreshProperties()
{
    MENUITEMINFO Info;
    FillStruct(&Info);
    SetMenuItemInfoW(_Owner->GetHandle(), _Handle, FALSE, &Info);

    HBITMAP Checked = _CheckedBitmap ? _CheckedBitmap->GetBitmap() : NULL;
    HBITMAP Unchecked = _UncheckedBitmap ? _UncheckedBitmap->GetBitmap() : NULL;
    SetMenuItemBitmaps(_Owner->GetHandle(), _Handle, MF_BYCOMMAND, Checked, Unchecked);
    _Owner->ItemChanged();
    //int Index = GetSystemMetrics(SM_CXMENUCHECK);
}

WinBitmap::Pointer WinMenuItem::CheckBitmap(WinBitmap::Pointer Bitmap)
{
    if (Bitmap)
    {
        int w = GetSystemMetrics(SM_CXMENUCHECK);
        int h = GetSystemMetrics(SM_CYMENUCHECK);
        if (Bitmap->GetWidth() == w && Bitmap->GetHeight() == h)
        {
            return Bitmap;
        }
        else
        {
            WinBitmap::Pointer DIB = std::make_shared<WinBitmap>(w, h, WinBitmap::BitmapBits::Bit32, false);
            DIB->GetWinDC()->Draw(0, 0, w, h, Bitmap);
            return DIB;
        }
    }
    return 0;
}

WinMenuItem::WinMenuItem(WinMenu* Owner, int Handle)
    : _Owner(Owner)
    , _Handle(Handle)
    , _SubMenu(nullptr)
    , _CheckedBitmap()
    , _UncheckedBitmap()
    , _Checked(false)
    , _Separator(false)
    , _RadioCheck(false)
    , _RadioGroup(-1)
    , _Enabled(true)
    , _Highlighted(false)
    , _Text()
    , _MenuText()
{
    _Owner->_MessageDispatcher->RegisterMenuItem(_Handle, this);
}

WinMenuItem::~WinMenuItem()
{
    _Owner->_MessageDispatcher->UnregisterMenuItem(_Handle);
    if (_SubMenu)
    {
        delete _SubMenu;
    }
}

void WinMenuItem::SetChecked(bool Value)
{
    _Checked = Value;
    if (_Checked && _RadioCheck)
    {
        for (WinMenuItem* Item : *_Owner)
        {
            if (Item->_Checked && Item->_RadioCheck && Item->_RadioGroup == _RadioGroup && Item != this)
            {
                Item->SetChecked(false);
            }
        }
    }
    RefreshProperties();
}

WinPopupMenu* WinMenuItem::CreateSubMenu()
{
    if (!_SubMenu)
    {
        _SubMenu = new WinPopupMenu(0);
        _SubMenu->_MessageDispatcher = _Owner->_MessageDispatcher;
        _SubMenu->_Associated = true;
        _SubMenu->_AssociatedMenuItem = this;
        RefreshProperties();
    }
    return _SubMenu;
}

void WinMenuItem::DestroySubMenu()
{
    if (_SubMenu)
    {
        _SubMenu->_Associated = false;
        delete _SubMenu;
        _SubMenu = nullptr;
        RefreshProperties();
    }
}

void WinFormMenu::ItemChanged()
{
    if (_Parent)
    {
        DrawMenuBar(_Parent->GetHandle());
    }
}

WinFormMenu::WinFormMenu(WinForm* Owner)
    : WinMenu(Owner)
    , _Parent(nullptr)
{
    _Handle = CreateMenu();
}

WinFormMenu::~WinFormMenu()
{
}

WinPopupMenu::WinPopupMenu(WinForm* Owner)
    : WinMenu(Owner)
{
    _Handle = CreatePopupMenu();
}

WinPopupMenu::~WinPopupMenu() {}

WinMenuItem* WinPopupMenu::Popup(int X, int Y, bool KeepPrevMenu /* = false*/)
{
    BOOL ID = TrackPopupMenuEx(_Handle,
                               TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | (KeepPrevMenu ? TPM_RECURSE : 0),
                               X,
                               Y,
                               _MessageDispatcher->GetHandle(),
                               NULL);
    WinMenuItem* Item = _MessageDispatcher->MenuItemO_Handle(ID);
    if (Item)
    {
        Item->OnClick(Item);
    }
    return Item;
}

WinMenuItem* WinPopupMenu::Popup(bool KeepPrevView /* = false*/)
{
    POINT Point;
    GetCursorPos(&Point);
    return Popup(Point.x, Point.y, KeepPrevView);
}

MsgBoxResult WinMsgBox(
    WinForm* Caller,
    const std::wstring& Text,
    const std::wstring& Caption,
    MsgBoxButton Button,
    MsgBoxIcon Icon/* = MsgBoxIcon::None*/,
    MsgBoxDefaultButton DefaultButton/* = MsgBoxDefaultButton::Button1*/
    )
{
    UINT Type = 0;
    switch (Button)
    {
    case MsgBoxButton::AbortRetryIgnore:
        Type = MB_ABORTRETRYIGNORE;
        break;
    case MsgBoxButton::OK:
        Type = MB_OK;
        break;
    case MsgBoxButton::OKCancel:
        Type = MB_OKCANCEL;
        break;
    case MsgBoxButton::RetryCancel:
        Type = MB_RETRYCANCEL;
        break;
    case MsgBoxButton::YesNo:
        Type = MB_YESNO;
        break;
    case MsgBoxButton::YesNoCancel:
        Type = MB_YESNOCANCEL;
        break;
    }
    switch (Icon)
    {
    case MsgBoxIcon::Exclamation:
        Type |= MB_ICONEXCLAMATION;
        break;
    case MsgBoxIcon::Information:
        Type |= MB_ICONINFORMATION;
        break;
    case MsgBoxIcon::Question:
        Type |= MB_ICONQUESTION;
        break;
    case MsgBoxIcon::Error:
        Type |= MB_ICONERROR;
        break;
    }
    switch (DefaultButton)
    {
    case MsgBoxDefaultButton::Button1:
        Type |= MB_DEFBUTTON1;
        break;
    case MsgBoxDefaultButton::Button2:
        Type |= MB_DEFBUTTON2;
        break;
    case MsgBoxDefaultButton::Button3:
        Type |= MB_DEFBUTTON3;
        break;
    case MsgBoxDefaultButton::Button4:
        Type |= MB_DEFBUTTON4;
        break;
    }
    Type |= MB_APPLMODAL;
    switch (MessageBoxW(Caller ? Caller->GetHandle() : NULL, Text.c_str(), Caption.c_str(), Type))
    {
    case IDABORT:
        return MsgBoxResult::Abort;
    case IDIGNORE:
        return MsgBoxResult::Ignore;
    case IDRETRY:
        return MsgBoxResult::Retry;
    case IDOK:
        return MsgBoxResult::OK;
    case IDCANCEL:
        return MsgBoxResult::Cancel;
    case IDYES:
        return MsgBoxResult::Yes;
    case IDNO:
        return MsgBoxResult::No;
    default:
        return MsgBoxResult::OK;
    }
}

void ShowMessage(WinForm* Caller, const std::wstring& Message)
{
    WinMsgBox(Caller, Message, GetApplication()->GetMainForm()->GetText(), MsgBoxButton::OK);
}

} //windows
} //pl
