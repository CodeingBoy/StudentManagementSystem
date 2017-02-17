#pragma once
#include <windows.h>
#include "ConsoleOperator.h"

#define DIALOG_RET_CONTINUE 0
#define DIALOG_RET_OK 1
#define DIALOG_RET_CANCEL 2
#define DIALOG_RET_YES 3
#define DIALOG_RET_NO 4

class Dialog
{
private:
    void DrawDialogBox();
protected:
    SMALL_RECT rect;
    SMALL_RECT clientArea;
    COORD size;
    ConsoleOperator& console;
public:
    Dialog(ConsoleOperator& console, int x, int y, int width, int height);
    Dialog(ConsoleOperator& console, int width, int height);
    Dialog(ConsoleOperator& console);
    ~Dialog();

    void DrawDialogBox(SMALL_RECT rect);
    void DrawDialogBorder();
    void DrawTitle(wstring title) const;

    int Show();
    virtual void Draw() = 0;
    virtual void Dispose() = 0;
    virtual int ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode) = 0;

    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();

    void SetPos(int x, int y, int width, int height);
    void SetCenteredPos(int width, int height);
    void RequestClientArea(int width, int height);
    void UpdateClientArea();

    static int GetMBCSLength(wstring str);
    int GetMBCSLength(wchar_t* str);
    static int GetWCSLength(string str);

};

