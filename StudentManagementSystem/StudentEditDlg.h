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
    bool addMode = false;
    Student student;
public:
    StudentEditDlg(ConsoleOperator& console);
    StudentEditDlg(ConsoleOperator& console, const Student& student);

    ~StudentEditDlg();

    void Draw() override;
    void Dispose() override;

    int ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode) override;

    Student GetStudent() const
    {
        return student;
    }
};

