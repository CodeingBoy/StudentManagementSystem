#include "AddStudentDlg.h"



AddStudentDlg::AddStudentDlg(ConsoleOperator& console): Dialog(console)
{
    SetCenteredPos(31, 10);
}


AddStudentDlg::~AddStudentDlg()
{
}

void AddStudentDlg::Draw()
{
    DrawTitle(_T("���ѧ����Ϣ"));

    console.WriteConsoleLine(_T("ѧ�ţ�"), { clientArea.Left, clientArea.Top + 1 });
    console.WriteConsoleLine(_T("������"), { clientArea.Left, clientArea.Top + 2 });
    console.WriteConsoleLine(_T("�Ա�"), { clientArea.Left, clientArea.Top + 3 });
    console.WriteConsoleLine(_T("�༶��"), { clientArea.Left, clientArea.Top + 4 });
    console.WriteConsoleLine(_T("��ϵ��ʽ��"), { clientArea.Left, clientArea.Top + 5 });

    SMALL_RECT editArea = { clientArea.Left + (clientArea.Right + 1 - clientArea.Left) / 2, // middle point
                            clientArea.Top + 1, clientArea.Right, clientArea.Top + 5
                          };
    console.FillArea(editArea, _T(' '), BACKGROUND_INTENSITY);

    // separator
    SMALL_RECT separator_rect = { clientArea.Left, clientArea.Bottom - 1, clientArea.Right, clientArea.Bottom - 1 };
    console.FillAreaChar(separator_rect, _T('��'));

    // draw button OK and Cancel
    DWORD fillNum;
    COORD btnOKPos = { clientArea.Left + 2, clientArea.Bottom };
    COORD btnCancelPos = { clientArea.Right - 2 - GetMBCSLength(cancelCaption), clientArea.Bottom };
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), OKCaption.c_str(), OKCaption.length(), btnOKPos, &fillNum);
    WriteConsoleOutputCharacter(console.GetConsoleHandle(), cancelCaption.c_str(), cancelCaption.length(), btnCancelPos, &fillNum);
}

void AddStudentDlg::Dispose()
{
}

