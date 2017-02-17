#include "InputDialog.h"
#include "StudentEditDlg.h"


InputDialog::InputDialog(ConsoleOperator& console, wstring title, wstring tips, wstring initalContent, short column, short row)
    : Dialog(console), title(title), tips(tips), initalContent(initalContent), inputarea_col(column), inputarea_row(row)
{
    tips_row = ceil(GetMBCSLength(tips) / static_cast<double>(inputarea_col));
    RequestClientArea(column + 2, 3 + inputarea_row + tips_row);
}


InputDialog::~InputDialog()
{
}

void InputDialog::Draw()
{
    DrawTitle(title);

    DWORD fillNum;

    // show content
    // console.WriteConsoleLine(tips, { clientArea.Left, clientArea.Top + 1 });
    COORD contentPos = { clientArea.Left + 1, clientArea.Top + 1 };
    SMALL_RECT content_line_rect = { clientArea.Left + 1, clientArea.Top + 1,
                                     clientArea.Right - 1, clientArea.Top + tips_row
                                   };
    const wchar_t* buffer = tips.c_str();
    int content_width_line = content_line_rect.Right + 1 - content_line_rect.Left;
    int strPos = 0;
    while (strPos < tips.length()) {
        WriteConsoleOutputCharacter(console.GetConsoleHandle(), buffer + strPos,
                                    min(content_width_line / 2, tips.length() - strPos), contentPos, &fillNum);
        strPos += min(content_width_line / 2, tips.length());
        contentPos.Y++;
    }

    editArea = { clientArea.Left + 1, content_line_rect.Bottom + 2,
                 clientArea.Left + inputarea_col, content_line_rect.Bottom + 1 + inputarea_row
               };
    console.FillArea(editArea, _T(' '), BACKGROUND_GREEN | BACKGROUND_BLUE);

    // separator
    SMALL_RECT separator_rect = { clientArea.Left, clientArea.Bottom - 1, clientArea.Right, clientArea.Bottom - 1 };
    console.FillAreaChar(separator_rect, _T('©¥'));

    // draw button OK and Cancel
    COORD btnOKPos = { clientArea.Left + 2, clientArea.Bottom };
    COORD btnCancelPos = { clientArea.Right - 2 - GetMBCSLength(cancelCaption), clientArea.Bottom };
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), OKCaption.c_str(), OKCaption.length(), btnOKPos, &fillNum);
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), cancelCaption.c_str(), cancelCaption.length(), btnCancelPos, &fillNum);

    // set cursor to first editarea
    console.ShowCursor();
    console.SetCursorSize(100);
    COORD defaultPos = { editArea.Left, editArea.Top };
    console.SetCursorPos(defaultPos);

    // set inital content to editarea
    COORD startPos = { editArea.Left, editArea.Top };
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), initalContent.c_str(), initalContent.length(), startPos, &fillNum);
}

void InputDialog::Dispose()
{
}

int InputDialog::ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode)
{
    SMALL_RECT editarea_rect = editArea;

    COORD curPos = console.GetCursorPos();
    DWORD fillNum;

    if (keyCode == keyCode_OK) { // OK
        const int col_count = inputarea_col;
        CHAR_INFO* pInfo = new CHAR_INFO[inputarea_row * inputarea_col];
        ReadConsoleOutput(console.GetConsoleHandle(), pInfo, { inputarea_col, inputarea_row }, { 0, 0 }, &editarea_rect);

        inputContent = ParseCharInfos(pInfo, 0, inputarea_row * inputarea_col - 1);

        delete[] pInfo;

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
    } else if ((keyCode == VK_DOWN && curPos.Y < editarea_rect.Bottom)) { // down arrow
        console.SetCursorPos({ editarea_rect.Left, curPos.Y + 1 });
    }

    if (keyCode > 0x00 && keyCode < 0x2f) { // input can't be printed
        return DIALOG_RET_CONTINUE;
    }

    wchar_t buffer[10] = { 0 };
    buffer[0] = keyEvent.uChar.UnicodeChar;
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), buffer, 1, curPos, &fillNum);
    if (curPos.X < editarea_rect.Right) {
        console.SetCursorPos({ static_cast<SHORT>(curPos.X + GetMBCSLength(buffer)), curPos.Y });
    }

    return DIALOG_RET_CONTINUE;
}

wstring InputDialog::ParseCharInfos(const CHAR_INFO const charinfos[], int begin, int end)
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