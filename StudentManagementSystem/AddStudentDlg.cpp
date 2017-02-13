#include "AddStudentDlg.h"



AddStudentDlg::AddStudentDlg(ConsoleOperator& console): Dialog(console)
{
    SetCenteredPos(31, 10);
}


AddStudentDlg::~AddStudentDlg()
{
}

void AddStudentDlg::Draw()
{
    DrawTitle(_T("添加学生信息"));

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

        if (keyCode == VK_UP && curPos.Y > defaultPos.Y) { // up
            console.SetCursorPos({defaultPos.X, curPos.Y - 1 });
        } else if ((keyCode == VK_DOWN && curPos.Y < defaultPos.Y + 5 - 1)) { // down
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

}

void AddStudentDlg::Dispose()
{
}

