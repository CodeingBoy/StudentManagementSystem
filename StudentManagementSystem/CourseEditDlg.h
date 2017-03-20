#pragma once
#include "ConsoleOperator.h"
#include "Dialog.h"
#include "Course.h"

class CourseEditDlg : public Dialog
{
private:
    ConsoleOperator &console = ConsoleOperator::GetInstance();
    const wstring OKCaption = _T("(Enter)确认"), cancelCaption = _T("(Esc)取消");
    const WORD keyCode_OK = VK_RETURN, keyCode_cancel = VK_ESCAPE;
    bool addMode = false;
    Course course;
public:
    CourseEditDlg(ConsoleOperator &console);
    CourseEditDlg(ConsoleOperator &console, const Course &course);

    ~CourseEditDlg();

    void Draw() override;
    void Dispose() override;

    int ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode) override;

    Course GetCourse() const
    {
        return course;
    }
};

