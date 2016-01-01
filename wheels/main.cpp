#include "WinGUI.h"

using namespace pl;
using namespace pl::windows;
using namespace pl::windows::placement;

/*���ڱ���*/
wchar_t* WindowTitle = L"debugger";

/*������*/
class RegForm: public WinForm
{
protected:
    /*�ؼ�*/
    WinEdit* txtRegex;
    WinEdit* txtRegex2;
    /*�ؼ���ʼ������*/
    void InitControls()
    {
        /*��ʼ���ؼ�*/
        txtRegex = new WinEdit(this, false);
        txtRegex2 = new WinEdit(this, false);
        ApplyPlacement(
            HorzScale(10, 100, 0.5,
                      Control(txtRegex),
                      Control(txtRegex2)
                      )
            );
        /*���ÿؼ��¼�*/
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