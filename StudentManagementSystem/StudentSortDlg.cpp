#include "StudentSortDlg.h"
#include <iostream>
#include "InputDialog.h"


StudentSortDlg::StudentSortDlg(ConsoleOperator& console) : Dialog(console)
{
    SetCenteredPos(37, 12);
}

StudentSortDlg::~StudentSortDlg()
{
}

void StudentSortDlg::Draw()
{
    DrawTitle(_T("排序学生信息"));

    console.WriteConsoleLine(_T("请输入排序权重值，留空为不排序"), { clientArea.Left, clientArea.Top + 1 });

    console.WriteConsoleLine(_T("学号："), { clientArea.Left, clientArea.Top + 3 });
    console.WriteConsoleLine(_T("姓名："), { clientArea.Left, clientArea.Top + 4 });
    console.WriteConsoleLine(_T("性别："), { clientArea.Left, clientArea.Top + 5 });
    console.WriteConsoleLine(_T("班级："), { clientArea.Left, clientArea.Top + 6 });
    console.WriteConsoleLine(_T("联系方式："), { clientArea.Left, clientArea.Top + 7 });

    SMALL_RECT editArea = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, // middle point
                            clientArea.Top + 3, clientArea.Right, clientArea.Top + 7
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
    COORD defaultPos = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, clientArea.Top + 3 };
    console.SetCursorPos(defaultPos);
}

int StudentSortDlg::ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode)
{
    SMALL_RECT editarea_rect = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, clientArea.Top + 3,
                                 clientArea.Right, clientArea.Top + 7
                               };
    COORD curPos = console.GetCursorPos();
    DWORD fillNum;

    if (keyCode == keyCode_OK) { // OK
        const int col_count = 14;
        CHAR_INFO info[5 * col_count];
        ReadConsoleOutput(console.GetConsoleHandle(), info, { 14, 5 }, { 0, 0 }, &editarea_rect);

        wstring ID, name, sex, clazz, phoneNum;
        ID = InputDialog::ParseCharInfos(info, 0, col_count - 1);
        name = InputDialog::ParseCharInfos(info, col_count, 2 * col_count - 1);
        sex = InputDialog::ParseCharInfos(info, 2 * col_count, 3 * col_count - 1);
        clazz = InputDialog::ParseCharInfos(info, 3 * col_count, 4 * col_count - 1);
        phoneNum = InputDialog::ParseCharInfos(info, 4 * col_count, 5 * col_count - 1);

        weight[0] = ID.empty() ? 0 : _wtoi(ID.c_str());
        weight[1] = name.empty() ? 0 : _wtoi(name.c_str());
        weight[2] = sex.empty() ? 0 : _wtoi(sex.c_str());
        weight[3] = clazz.empty() ? 0 : _wtoi(clazz.c_str());
        weight[4] = phoneNum.empty() ? 0 : _wtoi(phoneNum.c_str());

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
void StudentSortDlg::Dispose()
{
}

void StudentSortDlg::GetWeight(int outs[])
{
    outs[0] = weight[0];
    outs[1] = weight[1];
    outs[2] = weight[2];
    outs[3] = weight[3];
    outs[4] = weight[4];
}