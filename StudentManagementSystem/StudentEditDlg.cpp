#include "StudentEditDlg.h"
#include <iostream>


StudentEditDlg::StudentEditDlg(ConsoleOperator& console): Dialog(console), addMode(true)
{
    SetCenteredPos(31, 10);
}

StudentEditDlg::StudentEditDlg(ConsoleOperator& console, const Student& student)
    : Dialog(console),
      student(student)
{
    SetCenteredPos(31, 10);
}

StudentEditDlg::~StudentEditDlg()
{
}

void StudentEditDlg::Draw()
{
    if (addMode) {
        DrawTitle(_T("添加学生信息"));
    } else {
        DrawTitle(_T("编辑学生信息"));
    }

    console.WriteConsoleLine(_T("学号："), { clientArea.Left, clientArea.Top + 1 });
    console.WriteConsoleLine(_T("姓名："), { clientArea.Left, clientArea.Top + 2 });
    console.WriteConsoleLine(_T("性别："), { clientArea.Left, clientArea.Top + 3 });
    console.WriteConsoleLine(_T("班级："), { clientArea.Left, clientArea.Top + 4 });
    console.WriteConsoleLine(_T("联系方式："), { clientArea.Left, clientArea.Top + 5 });

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
    if (!addMode) {
        COORD startPos = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, clientArea.Top + 1 };
        WriteConsoleOutputCharacter(console.GetConsoleHandle(), student.GetID().c_str(), student.GetID().length(), startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), student.GetName().c_str(), student.GetName().length(), startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), student.IsMale() ? _T("男") : _T("女"), 2, startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), student.GetClass().c_str(), student.GetClass().length(), startPos, &fillNum);
        startPos.Y++;

        WriteConsoleOutputCharacter(console.GetConsoleHandle(), student.GetPhoneNum().c_str(), student.GetPhoneNum().length(), startPos, &fillNum);
    }

    SMALL_RECT editarea_rect = { defaultPos.X, defaultPos.Y, clientArea.Right, defaultPos.Y + 5 - 1 };
    KEY_EVENT_RECORD keyEvent;
    while (true) {
        keyEvent = console.GetKeyDownEvent();

        WORD keyCode = keyEvent.wVirtualKeyCode;
        COORD curPos = console.GetCursorPos();

        if ((keyCode == keyCode_OK) || (keyCode == keyCode_cancel)) { // OK or Cancel
            break;
        }

        if (keyCode == VK_BACK) { // backspace
            if (curPos.X >= defaultPos.X) {
                WriteConsoleOutputCharacter(console.GetConsoleHandle(), _T(" "), 1, curPos, &fillNum);
                if (curPos.X > defaultPos.X) {
                    console.SetCursorPos({ curPos.X - 1, curPos.Y });
                }
            }
            continue;
        }

        if (keyCode == VK_UP && curPos.Y > defaultPos.Y) { // up arrow
            console.SetCursorPos({defaultPos.X, curPos.Y - 1 });
        } else if ((keyCode == VK_DOWN && curPos.Y < defaultPos.Y + 5 - 1)) { // down arrow
            console.SetCursorPos({ defaultPos.X, curPos.Y + 1 });
        }

        if (keyCode > 0x00 && keyCode < 0x2f) { // input can't be printed
            continue;
        }

        wchar_t buffer[10] = {0};
        buffer[0] = keyEvent.uChar.UnicodeChar;
        WriteConsoleOutputCharacter(console.GetConsoleHandle(), buffer, 1, curPos, &fillNum);
        if (curPos.X < clientArea.Right) {
            console.SetCursorPos({ static_cast<SHORT>(curPos.X + GetMBCSLength(buffer)), curPos.Y });
        }
    }

    if (keyEvent.wVirtualKeyCode == keyCode_OK) { // add to list
        const int col_count = 14;
        CHAR_INFO info[5 * col_count];
        ReadConsoleOutput(console.GetConsoleHandle(), info, { 14, 5 }, { 0, 0 }, &editarea_rect);

        wstring ID, name, sex, clazz, phoneNum;
        ID = ParseCharInfos(info, 0, col_count - 1);
        name = ParseCharInfos(info, col_count, 2 * col_count - 1);
        sex = ParseCharInfos(info, 2 * col_count, 3 * col_count - 1);
        clazz = ParseCharInfos(info, 3 * col_count, 4 * col_count - 1);
        phoneNum = ParseCharInfos(info, 4 * col_count, 5 * col_count - 1);

        student.SetID(ID);
        student.SetName(name);
        student.SetIsMale(sex.compare(_T("男")) == 0);
        student.SetClass(clazz);
        student.SetPhoneNum(phoneNum);
    }
}

wstring StudentEditDlg::ParseCharInfos(const CHAR_INFO const charinfos[], int begin, int end)
{
    int col_count = end + 1 - begin;
    wchar_t* buffer = new wchar_t[col_count + 1];
    memset(buffer, 0, (col_count + 1) * sizeof(wchar_t));

    int offset = 0;
    for (int i = 0; i < col_count; ++i) {
        wchar_t uchar = charinfos[begin + i + offset].Char.UnicodeChar;
        if (uchar == _T(' '))break;
        buffer[i] = uchar;
        if (uchar != charinfos[begin + i + offset].Char.AsciiChar)++offset; // wide char
    }

    wstring result(buffer);
    delete[] buffer;
    return result;
}

void StudentEditDlg::Dispose()
{
}

