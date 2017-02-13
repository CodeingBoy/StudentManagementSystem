#pragma once
#include "ConsoleOperator.h"
#include "Dialog.h"
#include "Student.h"

class StudentEditDlg : public Dialog
{
private:
    ConsoleOperator& console = ConsoleOperator::GetInstance();
    const wstring OKCaption = _T("(Enter)确认"), cancelCaption = _T("(Esc)取消");
    const WORD keyCode_OK = VK_RETURN, keyCode_cancel = VK_ESCAPE;
    bool addMode = false;
    Student student;
public:
    StudentEditDlg(ConsoleOperator& console);
    StudentEditDlg(ConsoleOperator& console, const Student& student);

    ~StudentEditDlg();

    void Draw() override;
    void Dispose() override;

    static wstring StudentEditDlg::ParseCharInfos(const CHAR_INFO const charinfos[], int begin, int end);


    Student GetStudent() const
    {
        return student;
    }
};

