#include <iostream>
#include <TCHAR.h>
#include "Student.h"
#include "ConsoleOperator.h"
#include "ChoiceDlg.h"
#include "StudentManagementUI.h"
#include "defs.h"
#include "CourseManagementUI.h"

using namespace std;

ConsoleOperator& console = ConsoleOperator::GetInstance();
StudentManagementUI studentUI(console);
CourseManagementUI courseUI(console);

void Exit();
void OnExit();

int main()
{
    setlocale(LC_ALL, "chs");
    wcout.imbue(locale("chs"));
    console.SetTitle(_T("ѧ������ϵͳ")); // set window title

    while (true) {
        int retCode = studentUI.Show();
        switch (retCode) {
            case UI_RET_EXIT:
                Exit();
                break;
            case UI_RET_SWITCH:
                break;
        }

        retCode = courseUI.Show();
        switch (retCode) {
            case UI_RET_EXIT:
                Exit();
                break;
            case UI_RET_SWITCH:
                break;
        }
    }
}

void SaveToFile(wchar_t* fileName)
{
    CFileHandler handler(_T("data.txt"), false);
    StudentList& stuList = studentUI.GetStudentList();
    for (auto iter = stuList.begin(); iter != stuList.end(); ++iter) {
        Student& s = *iter;
        wstringstream wss;
        wss << s.GetID() << _T(",");
        wss << s.GetName() << _T(",");
        wss << (s.IsMale() ? _T("1") : _T("0")) << _T(",");
        wss << s.GetClass() << _T(",");
        wss << s.GetPhoneNum() << _T(",");

        handler.WriteLine(wss.str().c_str());
    }
}

void Exit()
{
    OnExit();
    exit(0);
}

void OnExit()
{
    ChoiceDlg confirm_dlg(console, _T("�Ƿ񱣴棿"), _T("�Ƿ񽫵�ǰϵͳ��Ϣ���浽�ļ���"), 40, _T("��Y������"), _T("��N������"));
    if (confirm_dlg.Show() == DIALOG_RET_YES) {
        SaveToFile(NULL);
    }
}

