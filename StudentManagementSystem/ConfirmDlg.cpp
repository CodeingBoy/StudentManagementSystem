#include "ConfirmDlg.h"



ConfirmDlg::ConfirmDlg(ConsoleOperator& console, wstring title, wstring content, int contentLineWidth, wstring btnOKCaption, WORD confirmKeyCode)
    : Dialog(console), title(title), content(content), contentLineWidth(contentLineWidth), btnOKCaption(btnOKCaption), confirmKeyCode(confirmKeyCode)
{
    // calc dialog width
    int len_title = ConsoleOperator::GetMBCSLength(title);
    int len_content = ConsoleOperator::GetMBCSLength(content);
    // width = border + space + width_content or width_title + space + border
    int width = 2 + 1 + max(len_title, len_content % contentLineWidth) + 1 + 2;
    // height = border + height_title + border + height_content + border + height_btnOK + border
    int height = 1 + 1 + 1 + ceil(static_cast<double>(len_content) / contentLineWidth) + 1 + 1 + 1;
    SetCenteredPos(width, height);
}


ConfirmDlg::~ConfirmDlg()
{
}

void ConfirmDlg::Draw()
{
    // separator
    SMALL_RECT separator_1_rect = { clientArea.Left, clientArea.Top + 1, clientArea.Right, clientArea.Top + 1 },
               separator_2_rect = { clientArea.Left, clientArea.Bottom - 1, clientArea.Right, clientArea.Bottom - 1};
    //console.FillAreaChar(separator_1_rect, _T('©¥'));
    console.FillAreaChar(separator_2_rect, _T('©¥'));

    DrawTitle(title); // draw title

    DWORD fillNum;
    // show content
    COORD contentPos = { clientArea.Left + 1, clientArea.Top + 1 };
    SMALL_RECT content_line_rect = { clientArea.Left + 1, clientArea.Top + 1, clientArea.Right - 1, clientArea.Top + 1 };
    const wchar_t* buffer = content.c_str();
    int content_width_line = content_line_rect.Right + 1 - content_line_rect.Left;
    int strPos = 0;
    while (strPos < content.length()) {
        WriteConsoleOutputCharacter(console.GetConsoleHandle(), buffer + strPos,
                                    min(content_width_line / 2, content.length() - strPos), contentPos, &fillNum);
        strPos += min(content_width_line / 2, content.length());
        contentPos.Y++;
    }

    // show button OK
    COORD btnOKPos = { size.X / 2 - GetMBCSLength(btnOKCaption) / 2, rect.Bottom - 1 };
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), btnOKCaption.c_str(), wcslen(btnOKCaption.c_str()), btnOKPos, &fillNum);

    WORD pressedKey;
    do {
        pressedKey = console.GetPressedDownKey();
    } while (pressedKey != confirmKeyCode);
}

void ConfirmDlg::Dispose()
{
}
