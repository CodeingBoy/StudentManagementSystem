#pragma once
#include "ConsoleOperator.h"
#include "Dialog.h"
#include "Student.h"

class StudentSearchDlg : public Dialog
{
private:
    ConsoleOperator& console = ConsoleOperator::GetInstance();
    const wstring OKCaption = _T("(Enter)确认"), cancelCaption = _T("(Esc)取消");
    const WORD keyCode_OK = VK_RETURN, keyCode_cancel = VK_ESCAPE;
    bool addMode = false;
    Student student;
    bool searchSex = false;
public:
    StudentSearchDlg(ConsoleOperator& console);
    StudentSearchDlg(ConsoleOperator& console, const Student& student);

    ~StudentSearchDlg();

    void Draw() override;
    void Dispose() override;
    bool IsAllEmpty();

    int ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode) override;

    Student GetStudent() const
    {
        return student;
    }

    bool GetSearchSex() const
    {
        return searchSex;
    }
};

