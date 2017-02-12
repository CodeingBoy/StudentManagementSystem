#pragma once
#include "ConsoleOperator.h"
#include "Dialog.h"

class AddStudentDlg : public Dialog
{
private:
    ConsoleOperator& console = ConsoleOperator::GetInstance();
    const wstring OKCaption = _T("<ȷ��>"), cancelCaption = _T("<ȡ��>");
public:
    AddStudentDlg(ConsoleOperator& console);
    ~AddStudentDlg();

    void Draw() override;
    void Dispose() override;
};

