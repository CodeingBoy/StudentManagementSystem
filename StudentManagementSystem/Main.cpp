#include <cstdio>
#include <iostream>
#include <TCHAR.h>
#include "Student.h"
#include "LinkedListSel.h"
#include "FileHandler.h"
#include "ConsoleOperator.h"
#include "ConfirmDlg.h"
#include "StudentEditDlg.h"
#include "ChoiceDlg.h"
#include "StudentManagementUI.h"

using namespace std;

ConsoleOperator& console = ConsoleOperator::GetInstance();
StudentManagementUI studentUI(console);

void OnExit();

int main()
{
    setlocale(LC_ALL, "chs");
    wcout.imbue(locale("chs"));
    console.SetTitle(_T("ѧ������ϵͳ")); // set window title

    if (studentUI.Show() == RET_OK) {
        OnExit();
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

void OnExit()
{
    ChoiceDlg confirm_dlg(console, _T("�Ƿ񱣴棿"), _T("�Ƿ񽫵�ǰϵͳ��Ϣ���浽�ļ���"), 40, _T("��Y������"), _T("��N������"));
    if (confirm_dlg.Show() == RET_OK) {
        //SaveToFile();
    }
}

