#pragma once
#include "ConsoleOperator.h"
#include "Dialog.h"
#include "Student.h"

class StudentSortDlg : public Dialog
{
private:
    ConsoleOperator& console = ConsoleOperator::GetInstance();
    const wstring OKCaption = _T("(Enter)»∑»œ"), cancelCaption = _T("(Esc)ª÷∏¥");
    const WORD keyCode_OK = VK_RETURN, keyCode_cancel = VK_ESCAPE;
    int weight[5];
public:
    StudentSortDlg(ConsoleOperator& console);
    StudentSortDlg(ConsoleOperator& console, const Student& student);
    ~StudentSortDlg();
    void Draw() override;
    void Dispose() override;
    bool IsAllEmpty();
    int ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode) override;
    void StudentSortDlg::GetWeight(int outs[]);
};

