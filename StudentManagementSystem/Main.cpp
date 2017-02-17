#include <iostream>
#include <TCHAR.h>
#include "Student.h"
#include "ConsoleOperator.h"
#include "ChoiceDlg.h"
#include "StudentManagementUI.h"
#include "defs.h"
#include "CourseManagementUI.h"
#include "StuCSVParser.h"
#include "InputDialog.h"

using namespace std;

ConsoleOperator& console = ConsoleOperator::GetInstance();
StudentManagementUI studentUI(console);
CourseManagementUI courseUI(console);
wstring readFileName;

void Exit();
void OnExit();
void ReadFromFile(wstring fileName);
void SaveToFile(wstring fileName);
void DrawVerticalRainbow();

int main()
{
    setlocale(LC_ALL, "chs");
    wcout.imbue(locale("chs"));
    console.SetTitle(_T("ѧ������ϵͳ")); // set window title
    COORD size = console.GetSize();
    console.SetSize({ size.X, 25 });

    DrawVerticalRainbow();
    InputDialog inputDlg(console, _T("��ȡ֮ǰ�ļ�¼"), _T("�������Ŀ¼�µ��ļ����Զ�ȡ�����ջ�ȡ��Ϊ����ȡ"));
    if (inputDlg.Show() == DIALOG_RET_OK) {
        wstring fileName = inputDlg.GetInputContent();
        if (!fileName.empty()) {
            ReadFromFile(fileName);
            readFileName = fileName;
        }
    }

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

void ReadFromFile(wstring fileName)
{
    CFileHandler handler(fileName.c_str(), true);
    CStuCSVParser parser(&handler);
    MyLinkedList<Student> list;
    parser.Parse(false, &list);

    StudentList& stuList = studentUI.GetStudentList();
    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        stuList.push_back(*iter);
    }
}

void SaveToFile(wstring fileName)
{
    CFileHandler handler(fileName.c_str(), false);
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
    ChoiceDlg confirm_dlg(console, _T("�Ƿ񱣴棿"), _T("�Ƿ񽫵�ǰϵͳ��Ϣ���浽�ļ���"), 40, _T("��Y������"), _T("��N������"));
    if (confirm_dlg.Show() == DIALOG_RET_YES) {
        InputDialog inputDlg(console, _T("�����¼"), _T("�������Ŀ¼�µ��ļ������ļ�·���Ա��棬���ջ�ȡ��Ϊ��������"), readFileName);
        if (inputDlg.Show() == DIALOG_RET_OK) {
            wstring fileName = inputDlg.GetInputContent();
            if (!fileName.empty()) {
                SaveToFile(fileName);
            }
        }
    }
}

void SetColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, ForeColor + BackGroundColor * 0x10);
}

// copied from others
void DrawVerticalRainbow()
{
    int i, j, k;
    for (i = 0; i < 25; ++i) {
        for (j = 0; j < 40; ++j) {
            k = j % 16;
            SetColor(k, k);
            putchar('A');
            putchar('A');
        }
    }
}
