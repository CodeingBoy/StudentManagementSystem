#pragma once
#include <windows.h>
#include "ConsoleOperator.h"

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

    void Show();
    virtual void Draw() = 0;
    virtual void Dispose() = 0;

    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();

    void SetPos(int x, int y, int width, int height);
    void SetCenteredPos(int width, int height);
    void UpdateClientArea();

    static int GetMBCSLength(wstring str);
    static int GetWCSLength(string str);

};

