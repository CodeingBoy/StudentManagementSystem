#include <cstdio>
#include <iostream>
#include <TCHAR.h>
#include "Student.h"
#include "LinkedListSel.h"
#include "FileHandler.h"
#include "ConsoleOperator.h"
#include "ConfirmDlg.h"
#include "StudentEditDlg.h"

#define ATTR_OPERINSTRUCTIONS   FOREGROUND_WHITE | BACKGROUND_RED
#define ATTR_STATUSBAR BACKGROUND_INTENSITY

using namespace std;

StudentList studentList;
ConsoleOperator& console = ConsoleOperator::GetInstance();

SMALL_RECT list_rect;

void AddStudent();
void RefreshList();
void SearchStudent();
void OnExit();

int main()
{
    setlocale(LC_ALL, "chs");
    wcout.imbue(locale("chs"));
    console.SetTitle(_T("学生管理系统")); // set window title
    console.HideCursor(); // hide cursor at first
    COORD size = console.GetSize();
    console.SetSize({ size.X, 25 });
    console.ClearScreen(FOREGROUND_WHITE | BACKGROUND_BLUE); // set background

    // set list rect
    list_rect = { 0, 1, size.X - 1, 21 };

    // show title
    console.SetLineAttr(0, FOREGROUND_WHITE | BACKGROUND_RED);
    console.WriteConsoleLine(_T("学生管理系统"), 0, NULL, ALIGN_CENTER);

    // show instructions
    console.SetLineAttr(22, ATTR_OPERINSTRUCTIONS);
    console.SetLineAttr(23, ATTR_OPERINSTRUCTIONS);

    console.WriteConsoleLine(_T("操作：（A）添加 （E）编辑 （D）删除 （S）搜索 (PgUp)上一屏 (PgDown)下一屏"), { 0, 22 });
    console.WriteConsoleLine(_T("      （C）查看学生选课 （M）管理学生选课 （Esc）退出"), { 0, 23 });


    // show status bar
    console.SetLineAttr(24, ATTR_STATUSBAR);
    console.WriteConsoleLine(_T("记录数：0┃文件：未读取┃状态：就绪"), 24, NULL, 1);

    RefreshList();

    // get input
    wchar_t input;
    bool shouldExit = false;
    while (!shouldExit && ((input = console.GetPressedDownKey()))) {
        switch (input) {
            case 0x41: { // A
                StudentEditDlg editDlg(console);
                editDlg.Show();
                studentList.push_back(editDlg.GetStudent());
                break;
            }
            case 0x45: { // E
                ConfirmDlg confirm_dlg(console, _T("无法添加"), _T("目前添加功能被禁用，请稍后再试。"), 20);
                confirm_dlg.Show();
                break;
            }
            case 0x44: // D
                shouldExit = true;
                break;
            case 0x53: // S
                shouldExit = true;
                break;
            case VK_PRIOR: // PgUp
                shouldExit = true;
                break;
            case VK_NEXT: // PgDown
                shouldExit = true;
                break;
            case VK_ESCAPE: // Esc
                OnExit();
                shouldExit = true;
                break;
            default:
                break;
        }
        console.HideCursor();
        RefreshList();
    }
}

void AddStudent()
{
    wstring id,  clazz, phoneNum;

    // ask for input
    wchar_t buffer[1024];
    wcout << "ID: ";
    wcin >> buffer;
    id = wstring(buffer);

    wcout << "Name: ";
    wcin >> buffer;
    wstring name(buffer);

    wcout << "Class: ";
    wcin >> buffer;
    clazz.assign(buffer);

    wcout << "PhoneNumber: ";
    wcin >> buffer;
    phoneNum.assign(buffer);

    // ask for confirm
    wcout << endl;
    wcout << "Here's what you have entered: " << endl;
    wcout << "ID:" << id << endl;
    wcout << "Name: " << name << endl;
    wcout << "Class: " << clazz << endl;
    wcout << "PhoneNumber: " << phoneNum << endl;
    wcout << endl;
    wcout << "Is that right? (y/n)";
    wcin.ignore(1024, '\n');

    bool confirmed = wcin.get() == _T('y');
    if (!confirmed) {
        wcout << "You have canceled adding student." << endl;
        return;
    }

    // add student to linked list
    Student student(id, name, clazz, true, phoneNum);
    studentList.push_back(student);

    wcout << "Successfully added student." << endl;
}

void DeleteStudent()
{
    wchar_t buffer[1024];

    wcout << "Pls input ID:";
    wcin >> buffer;


}

void RefreshList()
{
    static const short x_select = 0, x_ID = 8, x_name = 20, x_sex = 30, x_clazz = 38, x_phoneNum = 52;

    console.FillArea(list_rect, _T(' '), FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("┏━━━┳━━━━━┳━━━━┳━━━┳━━━━━━┳━━━━━━━━┓"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("┃ 选择 ┃   学号   ┃  姓名  ┃ 性别 ┃    班级    ┃    联系方式    ┃"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_BLUE);

    short y = 3;
    for (auto iter = studentList.begin(); iter != studentList.end(); ++iter) {
        Student s = *iter;
        console.WriteConsoleLine(_T("┃[   ]"), { x_select, y });
        console.WriteConsoleLine(_T("┃" + s.GetID()), { x_ID, y });
        console.WriteConsoleLine(_T("┃" + s.GetName()), { x_name, y });
        console.WriteConsoleLine(s.IsMale() ? _T("┃  男") : _T("┃  女"), { x_sex, y });
        console.WriteConsoleLine(_T("┃" + s.GetClass()), { x_clazz, y });
        console.WriteConsoleLine(_T("┃" + s.GetPhoneNum()), { x_phoneNum, y });

        y++;
        //console.WriteConsoleLine(output, { 0, 3 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
        /*wcout << "ID:" << s.GetID().c_str() << endl;
        wcout << "Name: " << s.GetName().c_str() << endl;
        wcout << "Class: " << s.GetClass().c_str() << endl;
        wcout << "PhoneNumber: " << s.GetPhoneNum().c_str() << endl;
        wcout << endl;*/
    }
}

void SearchStudent()
{
    wstring id, name, clazz, phoneNum;

    wcout << "Please input your search condition, leave blank to search all." << endl;

    // ask for input
    wchar_t buffer[1024];
    wcout << "ID: ";
    wcin >> buffer;
    id.assign(buffer);

    wcout << "Name: ";
    wcin >> buffer;
    name.assign(buffer);

    wcout << "Class: ";
    wcin >> buffer;
    clazz.assign(buffer);

    wcout << "PhoneNumber: ";
    wcin >> buffer;
    phoneNum.assign(buffer);

    Student condition(id, name, clazz, true, phoneNum);

    // search students
    MyLinkedList<Student> searchResult = studentList.Search_Fuzzy(condition, false);

    if (searchResult.size() != 0) {
        wcout << "Here's what you got:" << endl;

        for (auto iter = searchResult.begin(); iter != searchResult.end(); ++iter) {
            Student s = *iter;
            wcout << "ID:" << s.GetID().c_str() << endl;
            wcout << "Name: " << s.GetName().c_str() << endl;
            wcout << "Class: " << s.GetClass().c_str() << endl;
            wcout << "PhoneNumber: " << s.GetPhoneNum().c_str() << endl;
            wcout << endl;
        }
    } else {
        wcout << "All students are not matches your condition." << endl;
    }

    wcout << "==Search End==" << endl;
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

}