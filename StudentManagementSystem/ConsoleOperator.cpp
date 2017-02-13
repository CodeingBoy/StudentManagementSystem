#include "ConsoleOperator.h"
#include <algorithm>

ConsoleOperator::ConsoleOperator()
{
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    inputHandle = GetStdHandle(STD_INPUT_HANDLE);
}

ConsoleOperator::~ConsoleOperator()
{
    CloseHandle(consoleHandle);
}

wstring ConsoleOperator::GetTitle()
{
    wchar_t buffer[100];
    GetConsoleTitle(buffer, 100);
    return wstring(buffer);
}

void ConsoleOperator::SetTitle(wstring title)
{
    SetConsoleTitle(title.c_str());
}

DWORD ConsoleOperator::SetLineAttr(int coord_Y, WORD attr) const
{
    DWORD fillNum;
    COORD size = GetSize();
    FillConsoleOutputAttribute(consoleHandle, attr, size.X, { 0, static_cast<short>(coord_Y) }, &fillNum);

    return fillNum;
}

DWORD ConsoleOperator::WriteConsoleLine(wstring content, COORD writeCoord, WORD attr) const
{
    DWORD fillNum;

    if (attr != NULL) {
        FillConsoleOutputAttribute(consoleHandle, attr, GetMBCSLength(content), writeCoord, &fillNum);
    }

    WriteConsoleOutputCharacter(consoleHandle, content.c_str(), content.length(), writeCoord, &fillNum);

    return fillNum;
}

DWORD ConsoleOperator::WriteConsoleLine(wstring content, int coord_Y, WORD attr, int align) const
{
    DWORD fillNum;

    int coord_X;
    COORD size = GetSize();
    if (align == 0) { // left align
        coord_X = 0;
    } else if (align == 1) {
        coord_X = size.X - GetMBCSLength(content);
    } else { // right align
        coord_X = (size.X - GetMBCSLength(content)) / 2;
    }

    COORD writeCoord = { coord_X, coord_Y };

    return WriteConsoleLine(content, writeCoord, attr);
}

void ConsoleOperator::DrawDialogBox(SMALL_RECT rect)
{
    // draw shadows
    WORD bg_attr = BACKGROUND_INTENSITY;

    SMALL_RECT rect_shadow = { rect.Left + 1, rect.Top + 1, rect.Right + 1, rect.Bottom + 1 };
    FillAreaAttr(rect_shadow, bg_attr);

    // draw dialog plane
    WORD dlg_attr = FOREGROUND_WHITE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE; // do not use FOREGROUND_RED! -- is for text

    SMALL_RECT rect_dialog = { rect.Left, rect.Top, rect.Right, rect.Bottom };
    FillAreaAttr(rect_dialog, dlg_attr);
}

//void ConsoleOperator::ShowConfirmDialog(wstring title, wstring content, int lineStrCount, wstring btnOKCaption, WORD confirmKeyCode)
//{
//    // calc dialog width
//    int len_title = GetMBCSLength(title);
//    int len_content = GetMBCSLength(content);
//    // width = border + space + width_content or width_title + space + border
//    int width = 1 + 1 + max(len_title, len_content % lineStrCount) + 1 + 1;
//    // height = border + height_title + border + height_content + border + height_btnOK + border
//    int height = 1 + 1 + 1 + ceil(static_cast<double>(len_content) / lineStrCount) + 1 + 1 + 1;
//
//    // calc dialog pos
//    COORD size = GetSize();
//    SMALL_RECT dialogRect;
//
//    dialogRect.Left = size.X / 2 - width / 2; // middle - width / 2
//    dialogRect.Right = size.X / 2 + width / 2;
//
//    dialogRect.Top = size.Y / 2 - height / 2;
//    dialogRect.Bottom = size.Y / 2 + height / 2;
//
//    DrawDialogBox(dialogRect);
//
//    // horizontal separator
//    SMALL_RECT horizontalBorder_top = { dialogRect.Left, dialogRect.Top, dialogRect.Right, dialogRect.Top },
//               horizontalBorder_bottom = { dialogRect.Left, dialogRect.Bottom, dialogRect.Right, dialogRect.Bottom };
//    FillAreaChar(horizontalBorder_top, _T('-'));
//    FillAreaChar(horizontalBorder_bottom, _T('-'));
//
//    // vertical border
//    SMALL_RECT verticalBorder_left = { dialogRect.Left, dialogRect.Top, dialogRect.Left, dialogRect.Bottom };
//    FillAreaChar(verticalBorder_left, _T('|'));
//
//    SMALL_RECT verticalBorder_right = { dialogRect.Right, dialogRect.Top, dialogRect.Right, dialogRect.Bottom };
//    FillAreaChar(verticalBorder_right, _T('|'));
//
//    // separator
//    SMALL_RECT separator_1_rect = { dialogRect.Left + 1, dialogRect.Top + 2, dialogRect.Right - 1, dialogRect.Top + 2 },
//               separator_2_rect = { dialogRect.Left + 1, dialogRect.Bottom - 2, dialogRect.Right - 1, dialogRect.Bottom - 2 };
//    FillAreaChar(separator_1_rect, _T('-'));
//    FillAreaChar(separator_2_rect, _T('-'));
//
//    // show title
//    DWORD fillNum;
//
//    COORD dlgMidPos = { size.X / 2, (dialogRect.Bottom - dialogRect.Top) / 2 };
//    COORD titlePos = { dlgMidPos.X - GetMBCSLength(title) / 2, dialogRect.Top + 1 };
//    WriteConsoleOutputCharacter(consoleHandle, title.c_str(), wcslen(title.c_str()), titlePos, &fillNum);
//
//    // show content
//    COORD contentPos = { dialogRect.Left + 1, dialogRect.Top + 3 };
//    SMALL_RECT content_line_rect = { dialogRect.Left + 1, dialogRect.Top + 3, dialogRect.Right - 1, dialogRect.Top + 3};
//    const wchar_t* buffer = content.c_str();
//    int content_width_line = content_line_rect.Right + 1 - content_line_rect.Left;
//    int strPos = 0;
//    while (strPos < content.length()) {
//        WriteConsoleOutputCharacter(consoleHandle, buffer + strPos,
//                                    min(content_width_line / 2, content.length() - strPos), contentPos, &fillNum);
//        strPos += min(content_width_line / 2, content.length());
//        contentPos.Y++;
//    }
//
//    // show button OK
//    COORD btnOKPos = { dlgMidPos.X - GetMBCSLength(btnOKCaption) / 2, dialogRect.Bottom - 1 };
//    WriteConsoleOutputCharacter(consoleHandle, btnOKCaption.c_str(), wcslen(btnOKCaption.c_str()), btnOKPos, &fillNum);
//
//    WORD pressedKey;
//    do {
//        pressedKey = GetPressedKey();
//    } while (pressedKey != confirmKeyCode);
//}

void ConsoleOperator::FillScreen(wchar_t fillChar)
{
    COORD size = GetSize();
    COORD start = { 0, 0 };
    DWORD fillNum;
    FillConsoleOutputCharacter(consoleHandle, fillChar, size.X * size.Y, start, &fillNum);
}

void ConsoleOperator::FillScreen(wchar_t fillChar, WORD attr)
{
    COORD size = GetSize();
    COORD start = { 0, 0 };
    DWORD fillNum;
    FillConsoleOutputAttribute(consoleHandle, attr, size.X * size.Y, start, &fillNum);
    FillConsoleOutputCharacter(consoleHandle, fillChar, size.X * size.Y, start, &fillNum);
}

void ConsoleOperator::FillArea(SMALL_RECT rect, wchar_t fillChar, WORD attr)
{
    DWORD fillNum;
    COORD startPos = { rect.Left, rect.Top};
    int length = rect.Right + 1 - rect.Left;
    while (startPos.Y <= rect.Bottom) {
        if (attr != -1) {
            FillConsoleOutputAttribute(consoleHandle, attr, length, startPos, &fillNum);
        }
        FillConsoleOutputCharacter(consoleHandle, fillChar, length, startPos, &fillNum);
        startPos.Y++; // switch to next line
    }
}

void ConsoleOperator::FillAreaChar(SMALL_RECT rect, wchar_t fillChar) const
{
    DWORD fillNum;
    COORD startPos = { rect.Left, rect.Top };
    int length = rect.Right + 1 - rect.Left;
    while (startPos.Y <= rect.Bottom) {
        FillConsoleOutputCharacter(consoleHandle, fillChar, length, startPos, &fillNum);
        startPos.Y++; // switch to next line
    }
}

void ConsoleOperator::FillAreaAttr(SMALL_RECT rect, WORD attr) const
{
    DWORD fillNum;
    COORD startPos = { rect.Left, rect.Top };
    int length = rect.Right + 1 - rect.Left;
    while (startPos.Y <= rect.Bottom) {
        FillConsoleOutputAttribute(consoleHandle, attr, length, startPos, &fillNum);
        startPos.Y++; // switch to next line
    }
}

void ConsoleOperator::ClearScreen()
{
    FillScreen(_T(' '));
}

void ConsoleOperator::ClearScreen(WORD attr)
{
    FillScreen(_T(' '), attr);
}

COORD ConsoleOperator::GetCursorPos() const
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(consoleHandle, &info);
    return info.dwCursorPosition;
}

COORD ConsoleOperator::GetSize() const
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(consoleHandle, &info);
    return info.dwSize;
}

void ConsoleOperator::SetSize(COORD size) const
{
    SetConsoleScreenBufferSize(consoleHandle, size);
}


SMALL_RECT ConsoleOperator::GetWindowSize() const
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(consoleHandle, &info);
    return info.srWindow;
}

WORD ConsoleOperator::GetPressedDownKey()
{
    INPUT_RECORD record;
    DWORD readNum;
    while (true) {
        ReadConsoleInput(inputHandle, &record, 1, &readNum);
        if (record.EventType == KEY_EVENT) {
            KEY_EVENT_RECORD keyRecord = record.Event.KeyEvent;
            if (keyRecord.bKeyDown) {
                return keyRecord.wVirtualKeyCode;
            }
        }
    }
}

KEY_EVENT_RECORD ConsoleOperator::GetKeyDownEvent()
{
    INPUT_RECORD record;
    DWORD readNum;
    while (true) {
        ReadConsoleInput(inputHandle, &record, 1, &readNum);
        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
            return record.Event.KeyEvent;
        }
    }
}

void ConsoleOperator::SetCurrentTextAttribute(WORD attr)
{
    SetConsoleTextAttribute(consoleHandle, attr);
}

void ConsoleOperator::SetCursorPos(COORD pos)
{
    SetConsoleCursorPosition(consoleHandle, pos);
}

void ConsoleOperator::HideCursor()
{
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(consoleHandle, &info);

    info.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void ConsoleOperator::ShowCursor()
{
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(consoleHandle, &info);

    info.bVisible = true;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void ConsoleOperator::SetCursorSize(int size)
{
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(consoleHandle, &info);

    info.dwSize = size;
    SetConsoleCursorInfo(consoleHandle, &info);
}

int ConsoleOperator::GetMBCSLength(wstring str)
{
    return WideCharToMultiByte(CP_OEMCP, NULL, str.c_str(), -1, NULL, 0, NULL, FALSE);
}

int ConsoleOperator::GetWCSLength(string str)
{
    return MultiByteToWideChar(CP_OEMCP, NULL, str.c_str(), -1, NULL, 0);
}