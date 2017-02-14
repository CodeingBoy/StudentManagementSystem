#include <iostream>
#include <TCHAR.h>
#include "Student.h"
#include "ConsoleOperator.h"
#include "ChoiceDlg.h"
#include "StudentManagementUI.h"
#include "defs.h"
#include "CourseManagementUI.h"
#include "StuCSVParser.h"

using namespace std;

ConsoleOperator& console = ConsoleOperator::GetInstance();
StudentManagementUI studentUI(console);
CourseManagementUI courseUI(console);

void Exit();
void OnExit();
void ReadFromFile(wchar_t* fileName);

int main()
{
    setlocale(LC_ALL, "chs");
    wcout.imbue(locale("chs"));
    console.SetTitle(_T("学生管理系统")); // set window title

    ReadFromFile(NULL);

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

void ReadFromFile(wchar_t* fileName)
{
    CFileHandler handler(_T("data.txt"), true);
    CStuCSVParser parser(&handler);
    MyLinkedList<Student> list;
    parser.Parse(false, &list);

    StudentList& stuList = studentUI.GetStudentList();
    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        stuList.push_back(*iter);
    }
}

void SaveToFile(wchar_t* fileName)
{
    CFileHandler handler(_T("data.txt"), false);
    StudentList& stuList = studentUI.GetStudentList();
    for (auto iter = stuList.begin(); iter != stuList.end(); ++iter) {
        Student& s = *iter;
        s.GetFormatted(); // fill space instead of blank, so wcstok_s won't jump duplicate tokens

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
    ChoiceDlg confirm_dlg(console, _T("是否保存？"), _T("是否将当前系统信息保存到文件？"), 40, _T("（Y）保存"), _T("（N）舍弃"));
    if (confirm_dlg.Show() == DIALOG_RET_YES) {
        SaveToFile(NULL);
    }
}

