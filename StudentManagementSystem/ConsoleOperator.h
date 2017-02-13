#pragma once

#include <string>
#include <windows.h>
#include <TCHAR.h>

using namespace std;

#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2

class ConsoleOperator
{
private:
    HANDLE consoleHandle, inputHandle;
    ConsoleOperator();
    ~ConsoleOperator();
public:
    static ConsoleOperator& GetInstance()
    {
        static ConsoleOperator instance;
        return instance;
    }

    static wstring GetTitle();
    void SetTitle(wstring title);
    DWORD SetLineAttr(int coord_Y, WORD attr) const;
    DWORD WriteConsoleLine(wstring content, COORD writeCoord, WORD attr = NULL) const;
    DWORD WriteConsoleLine(wstring content, int coord_Y, WORD attr = NULL, int align = 0) const;
    void DrawDialogBox(SMALL_RECT rect);
    void ShowConfirmDialog(wstring title, wstring content, int lineStrCount, wstring btnOKCaption = _T("OK"), WORD confirmKeyCode = VK_RETURN);
    bool ShowYesNoDialog(wstring title, wstring content, wstring btnYesCaption = _T("Yes"), wstring btnNoCaption = _T("No"));
    void FillScreen(wchar_t fillChar);
    void FillScreen(wchar_t fillChar, WORD attr);
    void FillArea(SMALL_RECT rect, wchar_t fillChar, WORD attr);
    void FillAreaChar(SMALL_RECT rect, wchar_t fillChar) const;
    void FillAreaAttr(SMALL_RECT rect, WORD attr) const;
    void ClearScreen();
    void ClearScreen(WORD attr);
    COORD GetCursorPos() const;
    COORD GetSize() const;
    void SetSize(COORD size) const;
    SMALL_RECT GetWindowSize() const;
    WORD GetPressedDownKey();
    KEY_EVENT_RECORD GetKeyDownEvent();
    void SetCurrentTextAttribute(WORD attr);
    void SetCursorPos(COORD pos);


    HANDLE GetConsoleHandle() const
    {
        return consoleHandle;
    }

    void SetConsoleHandle(const HANDLE console_handle)
    {
        consoleHandle = console_handle;
    }

    void ShadowWindowLine(wchar_t* str);
    void DrawBox(bool bSingle, SMALL_RECT rc);
    void HideCursor();
    void ShowCursor();
    void SetCursorSize(int size);
    static int GetMBCSLength(wstring str);
    static int GetWCSLength(string str);
};


