#pragma once
#include "ConsoleOperator.h"
#include "Dialog.h"
#include "Student.h"

class StudentEditDlg : public Dialog
{
private:
    ConsoleOperator& console = ConsoleOperator::GetInstance();
    const wstring OKCaption = _T("(Enter)ȷ��"), cancelCaption = _T("(Esc)ȡ��");
    const WORD keyCode_OK = VK_RETURN, keyCode_cancel = VK_ESCAPE;
    Student student;
public:
    StudentEditDlg(ConsoleOperator& console);
    ~StudentEditDlg();

    void Draw() override;
    void Dispose() override;


    Student GetStudent() const
    {
        return student;
    }
};

