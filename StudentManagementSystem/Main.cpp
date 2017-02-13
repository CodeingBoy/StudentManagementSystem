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
CourseManagementUI courseUI();

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

        retCode = studentUI.Show();
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
    /*std::wofstream file(fileName, ios::ate);
    for (auto iter = studentList.begin(); iter != studentList.end(); ++iter) {
        Student s = *iter;
        file << s << endl;
    }
    file.close();*/
}

void Exit()
{
    OnExit();
    exit(0);
}

void OnExit()
{
    ChoiceDlg confirm_dlg(console, _T("�Ƿ񱣴棿"), _T("�Ƿ񽫵�ǰϵͳ��Ϣ���浽�ļ���"), 40, _T("��Y������"), _T("��N������"));
    if (confirm_dlg.Show() == DIALOG_RET_OK) {
        //SaveToFile();
    }
}

