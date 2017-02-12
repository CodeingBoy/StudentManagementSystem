#include "ConsoleOperator.h"
#include <algorithm>

ConsoleOperator::ConsoleOperator()
{
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
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

void ConsoleOperator::ShowConfirmDialog(wstring title, wstring content, int lineStrCount, wstring btnOKCaption)
{
    // calc dialog width
    int len_title = wcslen(title.c_str());
    int len_content = wcslen(content.c_str());
    // width = border + space + width_content or width_title + space + border
    int width = 1 + 1 + max(len_title, len_content % lineStrCount) + 1 + 1;
    // height = border + height_title + border + height_content + border + height_btnOK + border
    int height = 1 + 1 + 1 + ceil(static_cast<double>(len_content) / lineStrCount) + 1 + 1 + 1;

    // calc dialog pos
    COORD size = GetSize();
    SMALL_RECT dialogRect;

    dialogRect.Left = size.X / 2 - width / 2; // middle - width / 2
    dialogRect.Right = size.X / 2 + width / 2;

    dialogRect.Top = size.Y / 2 - height / 2;
    dialogRect.Bottom = size.Y / 2 + height / 2;

    // create shadows
    WORD bg_attr = BACKGROUND_INTENSITY;
    DWORD fillNum;

    COORD shadowStartPos = { dialogRect.Left + 1, dialogRect.Top + 1 };
    while (shadowStartPos.Y <= dialogRect.Bottom + 1) {
        FillConsoleOutputAttribute(consoleHandle, bg_attr, width, shadowStartPos, &fillNum); // fill a line shadow
        shadowStartPos.Y++; // switch to next line
    }

    // create dialog
    WORD dlg_attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE; // do not use FOREGROUND_RED! -- is for text

    COORD dlgStartPos = { dialogRect.Left, dialogRect.Top};
    while (dlgStartPos.Y <= dialogRect.Bottom) {
        FillConsoleOutputAttribute(consoleHandle, dlg_attr, width, dlgStartPos, &fillNum); // fill a line shadow
        dlgStartPos.Y++; // switch to next line
    }

    // create border
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // horizontal
    COORD horizontalBorderTopPos = { dialogRect.Left, dialogRect.Top };
    FillConsoleOutputCharacter(consoleHandle, _T('-'), width, horizontalBorderTopPos, &fillNum);
    FillConsoleOutputCharacter(consoleHandle, _T('-'), width, { dialogRect.Left, dialogRect.Top + 2 }, &fillNum);
    FillConsoleOutputCharacter(consoleHandle, _T('-'), width, { dialogRect.Left, dialogRect.Bottom - 2 }, &fillNum);
    FillConsoleOutputCharacter(consoleHandle, _T('-'), width, { dialogRect.Left, dialogRect.Bottom }, &fillNum); // fill a line shadow

    // vertical
    COORD verticalBorderLeftPos = { dialogRect.Left, dialogRect.Top };
    while (verticalBorderLeftPos.Y <= dialogRect.Bottom) {
        FillConsoleOutputCharacter(consoleHandle, _T('|'), 1, verticalBorderLeftPos, &fillNum); // fill a line shadow
        FillConsoleOutputCharacter(consoleHandle, _T('|'), 1, { dialogRect.Right, verticalBorderLeftPos.Y}, &fillNum); // fill a line shadow
        verticalBorderLeftPos.Y++; // switch to next line
    }

    // show title
    COORD dlgMidPos = { size.X / 2, (dialogRect.Bottom - dialogRect.Top) / 2 };
    COORD titlePos = { dlgMidPos.X - wcslen(title.c_str()) / 2, dialogRect.Top + 1 };
    WriteConsoleOutputCharacter(consoleHandle, title.c_str(), wcslen(title.c_str()), titlePos, &fillNum);

    // show content
    COORD contentPos = { dlgMidPos.X - wcslen(content.c_str()) / 2, dialogRect.Top + 3 };
    WriteConsoleOutputCharacter(consoleHandle, content.c_str(), wcslen(content.c_str()), contentPos, &fillNum);

    // show button OK
    COORD btnOKPos = { dlgMidPos.X - wcslen(btnOKCaption.c_str()) / 2, dialogRect.Bottom - 1 };
    WriteConsoleOutputCharacter(consoleHandle, btnOKCaption.c_str(), wcslen(btnOKCaption.c_str()), btnOKPos, &fillNum);

}

bool ConsoleOperator::ShowYesNoDialog(wstring title, wstring content, wstring btnYesCaption, wstring btnNoCaption)
{
    return true;
}

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

void ConsoleOperator::ShadowWindowLine(wchar_t* str)
{
    SMALL_RECT rc;
    CONSOLE_SCREEN_BUFFER_INFO bInfo; // ���ڻ�������Ϣ
    WORD att0, att1, attText;
    DWORD num;
    int i, chNum = wcslen(str);
    GetConsoleScreenBufferInfo(consoleHandle, &bInfo); // ��ȡ���ڻ�������Ϣ
    // ������ʾ���ڴ�С��λ��
    rc.Left = (bInfo.dwSize.X - chNum) / 2 - 2;
    rc.Top = 8; // ԭ������д˴�ΪbInfo.dwSize.Y/2 - 2�������������DOS��Ļ�д�ֱ�������Ļ�������Ҫ�ѹ������������ܿ���,Ϊ�˷���Ͱ�����Ϊ10
    rc.Right = rc.Left + chNum + 4;
    rc.Bottom = rc.Top + 4;
    att0 = BACKGROUND_INTENSITY; // ��Ӱ����
    att1 = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE;// �ı�����
    attText = FOREGROUND_RED | FOREGROUND_INTENSITY; // �ı�����
    // ������ӰȻ�����
    COORD posShadow = { rc.Left + 1, rc.Top + 1 }, posText = { rc.Left, rc.Top };
    for (i = 0; i < 5; i++) {
        FillConsoleOutputAttribute(consoleHandle, att0, chNum + 4, posShadow, &num);
        posShadow.Y++;
    }
    for (i = 0; i < 5; i++) {
        FillConsoleOutputAttribute(consoleHandle, att1, chNum + 4, posText, &num);
        posText.Y++;
    }
    // д�ı��ͱ߿�
    posText.X = rc.Left + 2;
    posText.Y = rc.Top + 2;
    WriteConsoleOutputCharacter(consoleHandle, str, wcslen(str), posText, &num);
    DrawBox(true, rc);
    SetConsoleTextAttribute(consoleHandle, bInfo.wAttributes); // �ָ�ԭ��������
}

void ConsoleOperator::DrawBox(bool bSingle, SMALL_RECT rc) // �������ܣ����߿�
{
    wchar_t chBox[6];
    DWORD num;
    COORD pos;
    if (bSingle) {
        chBox[0] = 'A'; // ���Ͻǵ�
        chBox[1] = (char)0xbf; // ���Ͻǵ�
        chBox[2] = (char)0xc0; // ���½ǵ�
        chBox[3] = (char)0xd9; // ���½ǵ�
        chBox[4] = '-'; // ˮƽ
        chBox[5] = (char)0xb3; // ��ֱ
    } else {
        chBox[0] = (char)0xc9; // ���Ͻǵ�
        chBox[1] = (char)0xbb; // ���Ͻǵ�
        chBox[2] = (char)0xc8; // ���½ǵ�
        chBox[3] = (char)0xbc; // ���½ǵ�
        chBox[4] = (char)0xcd; // ˮƽ
        chBox[5] = (char)0xba; // ��ֱ
    }
    // ���߿���� �±߽�
    for (pos.X = rc.Left + 1; pos.X < rc.Right - 1; pos.X++) {
        pos.Y = rc.Top;
        // ���ϱ߽�
        WriteConsoleOutputCharacter(consoleHandle, &chBox[4], 1, pos, &num);
        // �����Ͻ�
        if (pos.X == rc.Left + 1) {
            pos.X--;
            WriteConsoleOutputCharacter(consoleHandle, &chBox[0], 1, pos, &num);
            pos.X++;
        }
        // �����Ͻ�
        if (pos.X == rc.Right - 2) {
            pos.X++;
            WriteConsoleOutputCharacter(consoleHandle, &chBox[1], 1, pos, &num);
            pos.X--;
        }
        pos.Y = rc.Bottom;
        // ���±߽�
        WriteConsoleOutputCharacter(consoleHandle, &chBox[4], 1, pos, &num);
        // �����½�
        if (pos.X == rc.Left + 1) {
            pos.X--;
            WriteConsoleOutputCharacter(consoleHandle, &chBox[2], 1, pos, &num);
            pos.X++;
        }
        // �����½�
        if (pos.X == rc.Right - 2) {
            pos.X++;
            WriteConsoleOutputCharacter(consoleHandle, &chBox[3], 1, pos, &num);
            pos.X--;
        }
    }
    // ���߿�����ұ߽�
    for (pos.Y = rc.Top + 1; pos.Y <= rc.Bottom - 1; pos.Y++) {
        pos.X = rc.Left;
        // ����߽�
        WriteConsoleOutputCharacter(consoleHandle, &chBox[5], 1, pos, &num);
        pos.X = rc.Right - 1;
        // ���ұ߽�
        WriteConsoleOutputCharacter(consoleHandle, &chBox[5], 1, pos, &num);
    }
}

int ConsoleOperator::GetMBCSLength(wstring str)
{
    return WideCharToMultiByte(CP_OEMCP, NULL, str.c_str(), -1, NULL, 0, NULL, FALSE);
}