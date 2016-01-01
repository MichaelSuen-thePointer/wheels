#include "WinGUI.h"

using namespace pl;
using namespace pl::windows;
using namespace pl::windows::placement;

/*窗口标题*/
wchar_t* WindowTitle = L"debugger";

/*主窗口*/
class RegForm: public WinForm
{
protected:
    /*控件*/
    WinEdit* txtRegex;
    WinEdit* txtRegex2;
    /*控件初始化函数*/
    void InitControls()
    {
        /*初始化控件*/
        txtRegex = new WinEdit(this, false);
        txtRegex2 = new WinEdit(this, false);
        ApplyPlacement(
            HorzScale(10, 100, 0.5,
                      Control(txtRegex),
                      Control(txtRegex2)
                      )
            );
        /*设置控件事件*/
        txtRegex->OnChanged.Bind(this, &RegForm::txtRegex_OnChanged);
        txtRegex->OnKeyDown.Bind(this, &RegForm::txtRegex_OnKeyDown);
    }


    void txtRegex_OnChanged(Object* Sender)
    {

    }

    void txtRegex_OnKeyDown(Object* Sender, KeyStruct KeyStruct)
    {

    }

public:
    RegForm():WinForm(true)
    {
        SetText(WindowTitle);
        InitControls();
        MoveCenter();
    }
};

void main()
{
    (new RegForm())->ShowMaximized();
    GetApplication()->Run();
}