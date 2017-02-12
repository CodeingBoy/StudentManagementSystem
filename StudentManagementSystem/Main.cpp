#include <cstdio>
#include <iostream>
#include <TCHAR.h>
#include "Student.h"
#include "LinkedListSel.h"
#include "FileHandler.h"
#include "ConsoleOperator.h"

#define ATTR_OPERINSTRUCTIONS   FOREGROUND_WHITE | BACKGROUND_RED
#define ATTR_STATUSBAR BACKGROUND_INTENSITY


using namespace std;

StudentList studentList;
ConsoleOperator& console = ConsoleOperator::GetInstance();

void AddStudent();
void ShowStudent();
void SearchStudent();

int main()
{
    setlocale(LC_ALL, "chs");
    wcout.imbue(locale("chs"));
    console.SetTitle(_T("学生管理系统")); // set window title
    COORD size = console.GetSize();
    console.SetSize({ size.X, 25 });
    console.ClearScreen(FOREGROUND_WHITE | BACKGROUND_BLUE); // set background

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

    ShowStudent();

    // get input
    wchar_t input;
    bool shouldExit = false;
    while (!shouldExit && ((input = console.GetInput()))) {
        switch (input) {
            case 0x41: // A
                shouldExit = true;
                break;
            case 0x45: // E
                shouldExit = true;
                break;
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
                shouldExit = true;
                break;
            default:
                break;
        }
    }
    //console.ShowConfirmDialog(_T("title"), _T("contentcontentcontentcontentcontent"), 10);
    //do {
    //    //wcout << "Welcome using Student Management System\n" << endl;
    //    printf("Please sel func:\n");
    //    printf("1. add\n");
    //    printf("2. del\n");
    //    printf("3. show\n");
    //    printf("4. search\n");

    //    wcin >> select;

    //    switch (select) {
    //        case 1:
    //            AddStudent();
    //            break;
    //        case 3:
    //            ShowStudent();
    //            break;
    //        case 4:
    //            SearchStudent();
    //            break;
    //        default:
    //            break;
    //    }
    //    //console.ClearScreen();
    //} while (select);
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

wstring GetFormattedString(Student s)
{
    wchar_t buffer[1024];
    swprintf_s(buffer, 1024, _T("┃ [ ]  ┃%-10s┃%-3s┃ %-1s ┃%-6s┃%-8s┃"),
               s.GetID().c_str(), s.GetName().c_str(), s.IsMale() ? _T("男") : _T("女"), s.GetClass().c_str(), s.GetPhoneNum().c_str());
    return wstring(buffer);
}

void ShowStudent()
{
    console.WriteConsoleLine(_T("┏━━━┳━━━━━┳━━━━┳━━━┳━━━━━━┳━━━━━━━━┓"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("┃ 选择 ┃   学号   ┃  姓名  ┃ 性别 ┃    班级    ┃    联系方式    ┃"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_BLUE);

    for (auto iter = studentList.begin(); iter != studentList.end(); ++iter) {
        Student s = *iter;
        wstring output = GetFormattedString(s);
        console.WriteConsoleLine(output, { 0, 3 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
        wcout << output << endl;
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