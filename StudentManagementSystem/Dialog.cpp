#include "Dialog.h"
#include "ConsoleOperator.h"


Dialog::Dialog(ConsoleOperator& console, int x, int y, int width, int height): console(console), size(console.GetSize())
{
    SetPos(x, y, width, height);
}

Dialog::Dialog(ConsoleOperator& console, int width, int height) : console(console), size(console.GetSize())
{
    SetCenteredPos(width, height);
}

Dialog::Dialog(ConsoleOperator& console): console(console), size(console.GetSize())
{
}


Dialog::~Dialog()
{
}


void Dialog::DrawDialogBox(SMALL_RECT rect)
{
    // draw shadows
    WORD bg_attr = BACKGROUND_INTENSITY;

    SMALL_RECT rect_shadow = { rect.Left + 1, rect.Top + 1, rect.Right + 1, rect.Bottom + 1 };
    console.FillArea(rect_shadow, _T(' '), bg_attr);

    // draw dialog plane
    WORD dlg_attr = FOREGROUND_WHITE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE; // do not use FOREGROUND_RED! -- is for text

    SMALL_RECT rect_dialog = { rect.Left, rect.Top, rect.Right, rect.Bottom };
    console.FillArea(rect_dialog, _T(' '), dlg_attr);
}

void Dialog::DrawDialogBorder()
{
    // horizontal border
    SMALL_RECT horizontalBorder_top = { rect.Left, rect.Top, rect.Right, rect.Top },
               horizontalBorder_bottom = { rect.Left, rect.Bottom, rect.Right, rect.Bottom };
    console.FillAreaChar(horizontalBorder_top, _T('©¥'));
    console.FillAreaChar(horizontalBorder_bottom, _T('©¥'));

    // vertical border
    SMALL_RECT verticalBorder_left = { rect.Left, rect.Top, rect.Left + 1, rect.Bottom };
    console.FillAreaChar(verticalBorder_left, _T('©§'));

    SMALL_RECT verticalBorder_right = { rect.Right - 1, rect.Top, rect.Right, rect.Bottom };
    console.FillAreaChar(verticalBorder_right, _T('©§'));

    // draw conner
    console.FillAreaChar({ rect.Left, rect.Top, rect.Left + 1, rect.Top }, _T('©³'));          // upper left
    console.FillAreaChar({ rect.Right - 1, rect.Top, rect.Right, rect.Top }, _T('©·'));         // upper right
    console.FillAreaChar({ rect.Left, rect.Bottom, rect.Left + 1, rect.Bottom }, _T('©»'));      // bottom left
    console.FillAreaChar({ rect.Right - 1, rect.Bottom, rect.Right, rect.Bottom }, _T('©¿'));  // bottom right
}

void Dialog::DrawTitle(wstring title) const
{
    // redraw upper border to clean previous title
    SMALL_RECT horizontalBorder_top = { rect.Left + 2, rect.Top, rect.Right - 2, rect.Top };
    console.FillAreaChar(horizontalBorder_top, _T('©¥'));

    DWORD fillNum;
    COORD titlePos = { size.X / 2 - GetMBCSLength(title) / 2, clientArea.Top - 1 }; // draw over border
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), title.c_str(), wcslen(title.c_str()), titlePos, &fillNum);
}

int Dialog::Show()
{
    DrawDialogBox(rect);  // draw dialog plane
    DrawDialogBorder();   // draw dialog border
    Draw();

    int retcode;
    do {
        KEY_EVENT_RECORD keyEvent;
        keyEvent = console.GetKeyDownEvent();
        WORD keyCode = keyEvent.wVirtualKeyCode;

        retcode = ProcessInput(keyEvent, keyCode);
    } while (retcode == DIALOG_RET_CONTINUE);
    return retcode;
}

int Dialog::GetX()
{
    return rect.Left;
}

int Dialog::GetY()
{
    return rect.Top;
}

int Dialog::GetWidth()
{
    return rect.Right + 1 - rect.Left;
}

int Dialog::GetHeight()
{
    return rect.Bottom + 1 - rect.Top;
}

void Dialog::SetPos(int x, int y, int width, int height)
{
    rect.Left = x;
    rect.Right = x + width;
    rect.Top = y;
    rect.Bottom = y + height;

    UpdateClientArea();
}

void Dialog::SetCenteredPos(int width, int height)
{
    COORD size = console.GetSize();
    rect.Left = (size.X - width) / 2;
    rect.Right = (size.X + width) / 2;

    rect.Top = (size.Y - height) / 2;
    rect.Bottom = (size.Y + height) / 2;

    UpdateClientArea();
}

void Dialog::UpdateClientArea()
{
    clientArea = { rect.Left + 2, rect.Top + 1, rect.Right - 2, rect.Bottom - 1 };
}

int Dialog::GetMBCSLength(wstring str)
{
    return WideCharToMultiByte(CP_OEMCP, NULL, str.c_str(), -1, NULL, 0, NULL, FALSE);
}

int Dialog::GetMBCSLength(wchar_t* str)
{
    return WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, 0, NULL, FALSE) - 1; // delete '\0'
}

int Dialog::GetWCSLength(string str)
{
    return MultiByteToWideChar(CP_OEMCP, NULL, str.c_str(), -1, NULL, 0);
}