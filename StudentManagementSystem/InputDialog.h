#pragma once
#include "Dialog.h"
class InputDialog :
    public Dialog
{
private:
    wstring title, tips, initalContent;
    wstring OKCaption = _T("��Enter��ȷ��"), cancelCaption = _T("��Esc��ȡ��");
    SMALL_RECT editArea;
    static const WORD keyCode_OK = VK_RETURN, keyCode_cancel = VK_ESCAPE;
    wstring inputContent;
    short inputarea_row, inputarea_col;
    int tips_row = 1;
public:
    InputDialog(ConsoleOperator& console, wstring title, wstring tips, wstring initalContent = _T(""), short column = 35, short row = 1);
    ~InputDialog();

    void Draw() override;
    void Dispose() override;
    int ProcessInput(KEY_EVENT_RECORD keyEvent, WORD keyCode) override;
    static wstring  ParseCharInfos(const CHAR_INFO const charinfos[], int begin, int end);

    wstring GetInputContent() const
    {
        return inputContent;
    }
};

