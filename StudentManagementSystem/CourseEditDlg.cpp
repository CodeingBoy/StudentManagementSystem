#include "CourseEditDlg.h"
#include <iostream>
#include "InputDialog.h"


CourseEditDlg::CourseEditDlg(ConsoleOperator &console) : Dialog(console), addMode(true)
{
    SetCenteredPos(31, 10);
}

CourseEditDlg::CourseEditDlg(ConsoleOperator &console, const Course &course)
    : Dialog(console),
      course(course)
{
    SetCenteredPos(31, 10);
}

CourseEditDlg::~CourseEditDlg()
{
}

void CourseEditDlg::Draw()
{
    if (addMode) {
        DrawTitle(_T("添加学生信息"));
    } else {
        DrawTitle(_T("编辑学生信息"));
    }

    console.WriteConsoleLine(_T("课程编号："), { clientArea.Left, clientArea.Top + 1 });
    console.WriteConsoleLine(_T("课程名："), { clientArea.Left, clientArea.Top + 2 });
    console.WriteConsoleLine(_T("开课学期："), { clientArea.Left, clientArea.Top + 3 });
    console.WriteConsoleLine(_T("任课教师："), { clientArea.Left, clientArea.Top + 4 });

    SMALL_RECT editArea = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, // middle point
                            clientArea.Top + 1, clientArea.Right, clientArea.Top + 5
                          };
    console.FillArea(editArea, _T(' '), BACKGROUND_GREEN | BACKGROUND_BLUE);

    // separator
    SMALL_RECT separator_rect = { clientArea.Left, clientArea.Bottom - 1, clientArea.Right, clientArea.Bottom - 1 };
    console.FillAreaChar(separator_rect, _T('━'));

    // draw button OK and Cancel
    DWORD fillNum;
    COORD btnOKPos = { clientArea.Left + 2, clientArea.Bottom };
    COORD btnCancelPos = { clientArea.Right - 2 - GetMBCSLength(cancelCaption), clientArea.Bottom };
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), OKCaption.c_str(), OKCaption.length(), btnOKPos, &fillNum);
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), cancelCaption.c_str(), cancelCaption.length(), btnCancelPos, &fillNum);

    // set cursor to first editarea
    console.ShowCursor();
    console.SetCursorSize(100);
    COORD defaultPos = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, clientArea.Top + 1 };
    console.SetCursorPos(defaultPos);

    // in edit mode, set student inf to editarea
    /*if (!addMode) {
        COORD startPos = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, clientArea.Top + 1 };
        WriteConsoleOutputCharacter(console.GetConsoleHandle(), course.GetID().c_str(), course.GetID().length(), startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), course.GetName().c_str(), course.GetName().length(), startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), course.IsMale() ? _T("男") : _T("女"), 2, startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), course.GetClass().c_str(), course.GetClass().length(), startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), course.GetPhoneNum().c_str(), course.GetPhoneNum().length(), startPos, &fillNum);
    }*/

    SMALL_RECT editarea_rect = { defaultPos.X, defaultPos.Y, clientArea.Right, defaultPos.Y + 5 - 1 };
}

int CourseEditDlg::ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode)
{
    SMALL_RECT editarea_rect = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, clientArea.Top + 1,
                                 clientArea.Right, clientArea.Top + 5
                               };
    COORD curPos = console.GetCursorPos();
    DWORD fillNum;

    if (keyCode == keyCode_OK) { // OK
        const int col_count = 14;
        CHAR_INFO info[5 * col_count];
        ReadConsoleOutput(console.GetConsoleHandle(), info, { 14, 5 }, { 0, 0 }, &editarea_rect);

        wstring ID, name, period, teacherName;
        ID = InputDialog::ParseCharInfos(info, 0, col_count - 1);
        name = InputDialog::ParseCharInfos(info, col_count, 2 * col_count - 1);
        period = InputDialog::ParseCharInfos(info, 2 * col_count, 3 * col_count - 1);
        teacherName = InputDialog::ParseCharInfos(info, 3 * col_count, 4 * col_count - 1);

        course.SetID(ID);
        course.SetName(name);
        course.SetPeriod(period);
        course.SetTeacherName(teacherName);

        return DIALOG_RET_OK;
    } else if (keyCode == keyCode_cancel) { // Cancel
        return DIALOG_RET_CANCEL;
    }

    if (keyCode == VK_BACK) { // backspace
        if (curPos.X >= editarea_rect.Left) {
            WriteConsoleOutputCharacter(console.GetConsoleHandle(), _T(" "), 1, curPos, &fillNum);
            if (curPos.X > editarea_rect.Left) {
                console.SetCursorPos({ curPos.X - 1, curPos.Y });
            }
        }
        return DIALOG_RET_CONTINUE;
    }

    if (keyCode == VK_UP && curPos.Y > editarea_rect.Top) { // up arrow
        console.SetCursorPos({ editarea_rect.Left, curPos.Y - 1 });
    } else if ((keyCode == VK_DOWN && curPos.Y < editarea_rect.Top + 5 - 1)) { // down arrow
        console.SetCursorPos({ editarea_rect.Left, curPos.Y + 1 });
    }

    if (keyCode > 0x00 && keyCode < 0x2f) { // input can't be printed
        return DIALOG_RET_CONTINUE;
    }

    wchar_t buffer[10] = { 0 };
    buffer[0] = keyEvent.uChar.UnicodeChar;
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), buffer, 1, curPos, &fillNum);
    if (curPos.X < clientArea.Right) {
        console.SetCursorPos({ static_cast<SHORT>(curPos.X + GetMBCSLength(buffer)), curPos.Y });
    }

    return DIALOG_RET_CONTINUE;
}
void CourseEditDlg::Dispose()
{
}

