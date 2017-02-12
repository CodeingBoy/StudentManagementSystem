#pragma once
#include "ConsoleOperator.h"
#include "Dialog.h"

class AddStudentDlg : public Dialog
{
private:
    ConsoleOperator& console = ConsoleOperator::GetInstance();
    const wstring OKCaption = _T("<确认>"), cancelCaption = _T("<取消>");
public:
    AddStudentDlg(ConsoleOperator& console);
    ~AddStudentDlg();

    void Draw() override;
    void Dispose() override;
};

