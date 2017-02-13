#pragma once
#include "Dialog.h"

class ChoiceDlg : public Dialog
{
private:
    wstring title;
    wstring content;
    int contentLineWidth;
    wstring btnYesCaption;
    wstring btnNoCaption;
public:
    ChoiceDlg();
    ChoiceDlg(ConsoleOperator& console, wstring title, wstring content, int contentLineWidth = 50,
              wstring btnYesCaption = _T("£¨Y£©ÊÇ"), wstring btnNoCaption = _T("£¨N£©·ñ"));
    ~ChoiceDlg();
    void Draw() override;
    void Dispose() override;
};

