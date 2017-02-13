#pragma once
#include "Dialog.h"
class ConfirmDlg :
    public Dialog
{
private:
    wstring title;
    wstring content;
    int contentLineWidth;
    wstring btnOKCaption;
    WORD confirmKeyCode;
public:
    ConfirmDlg(ConsoleOperator& console, wstring title, wstring content, int contentLineWidth,
               wstring btnOKCaption = _T("(Enter)OK"), WORD confirmKeyCode = VK_RETURN);
    ~ConfirmDlg();

    void Draw() override;
    void Dispose() override;
};

