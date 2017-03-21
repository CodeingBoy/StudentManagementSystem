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
#include "CourseCSVParser.h"
#include "ConfirmDlg.h"

using namespace std;

ConsoleOperator& console = ConsoleOperator::GetInstance();
StudentManagementUI studentUI(console);
CourseManagementUI courseUI(console);
wstring readFileName;

void Exit();
void OnExit();
bool ReadFromFile(wstring fileName);
void SaveToFile(wstring fileName);
void DrawVerticalRainbow();

int main()
{
    setlocale(LC_ALL, "chs");
    wcout.imbue(locale("chs"));
    console.SetTitle(_T("ѧ������ϵͳ")); // set window title

    COORD size = console.GetSize();
    console.SetSize({ 80, 25 });

    DrawVerticalRainbow();
    while (true) {
        InputDialog inputDlg(console, _T("��ȡ֮ǰ�ļ�¼"), _T("�������Ŀ¼�µ��ļ����Զ�ȡ�����ջ�ȡ��Ϊ����ȡ"));
        if (inputDlg.Show() != DIALOG_RET_OK) break;
        wstring fileName = inputDlg.GetInputContent();
        if (fileName.empty()) break;

        if(ReadFromFile(fileName)) {
            readFileName = fileName;
            break;
        }

        ConfirmDlg errorDlg(console, _T("�޷���ȡ"), _T("�ļ��޷���ȡ������Ϊ�Ҳ����ļ�����û�ж��ļ���ȡ��Ȩ�ޣ��������ṩ���ļ�·��"), 50);
        errorDlg.Show();
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

bool ReadFromFile(wstring fileName)
{
    CFileHandler handler(fileName.c_str(), true);
    if (!handler.IsVaild())return false;

    // parse student list
    CStuCSVParser parser_stu(&handler);
    MyLinkedList<Student> list;
    parser_stu.Parse(false, &list, _T("\n"));

    StudentList& stuList = studentUI.GetStudentList();
    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        stuList.push_back(*iter);
    }

    // parse course list
    CCourseCSVParser parser_course(&handler);
    MyLinkedList<Course>& course_list = courseUI.GetCourseList();
    parser_course.Parse(false, &course_list, _T("\n"));

    return true;
}

void SaveToFile(wstring fileName)
{
    CFileHandler handler(fileName.c_str(), false);
    StudentList& stuList = studentUI.GetStudentList();
    MyLinkedList<Course> courses = courseUI.GetCourseList();
    for (auto iter = stuList.begin(); iter != stuList.end(); ++iter) {
        Student& s = *iter;
        s.GetFormatted(); // fill space instead of blank, so wcstok_s won't jump duplicate tokens

        wstringstream wss;
        wss << s.GetID() << _T(",");
        wss << s.GetName() << _T(",");
        wss << (s.IsMale() ? _T("1") : _T("0")) << _T(",");
        wss << s.GetClass() << _T(",");
        wss << s.GetPhoneNum() << _T(",");
        MyLinkedList<wstring> chosen_course = s.GetChosenCourses();
        for(auto iter = chosen_course.begin(); iter != chosen_course.end(); ++iter) {
            wss << *iter << _T("|");
        }
        wss << _T(",");

        handler.WriteLine(wss.str().c_str());
    }

    handler.Write(_T("\n"));

    for (auto iter = courses.begin(); iter != courses.end(); ++iter) {
        Course& c = *iter;
        c.GetFormatted(); // fill space instead of blank, so wcstok_s won't jump duplicate tokens

        wstringstream wss;
        wss << c.GetID() << _T(",");
        wss << c.GetName() << _T(",");
        wss << c.GetPeriod() << _T(",");
        wss << c.GetTeacherName() << _T(",");

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

MyLinkedList<Course> GetCourse()
{
    return courseUI.GetCourseList();
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
