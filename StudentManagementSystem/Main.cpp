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
    console.SetTitle(_T("学生管理系统")); // set window title

    if (studentUI.Show() == RET_OK) {
        OnExit();
    }

}


//void AddStudent()
//{
//    wstring id,  clazz, phoneNum;
//
//    // ask for input
//    wchar_t buffer[1024];
//    wcout << "ID: ";
//    wcin >> buffer;
//    id = wstring(buffer);
//
//    wcout << "Name: ";
//    wcin >> buffer;
//    wstring name(buffer);
//
//    wcout << "Class: ";
//    wcin >> buffer;
//    clazz.assign(buffer);
//
//    wcout << "PhoneNumber: ";
//    wcin >> buffer;
//    phoneNum.assign(buffer);
//
//    // ask for confirm
//    wcout << endl;
//    wcout << "Here's what you have entered: " << endl;
//    wcout << "ID:" << id << endl;
//    wcout << "Name: " << name << endl;
//    wcout << "Class: " << clazz << endl;
//    wcout << "PhoneNumber: " << phoneNum << endl;
//    wcout << endl;
//    wcout << "Is that right? (y/n)";
//    wcin.ignore(1024, '\n');
//
//    bool confirmed = wcin.get() == _T('y');
//    if (!confirmed) {
//        wcout << "You have canceled adding student." << endl;
//        return;
//    }
//
//    // add student to linked list
//    Student student(id, name, clazz, true, phoneNum);
//    studentList.push_back(student);
//
//    wcout << "Successfully added student." << endl;
//}

//void DeleteStudent(int num)
//{
//    wchar_t buffer[1024];
//
//    wcout << "Pls input ID:";
//    wcin >> buffer;
//
//
//}

//void SearchStudent()
//{
//    wstring id, name, clazz, phoneNum;
//
//    wcout << "Please input your search condition, leave blank to search all." << endl;
//
//    // ask for input
//    wchar_t buffer[1024];
//    wcout << "ID: ";
//    wcin >> buffer;
//    id.assign(buffer);
//
//    wcout << "Name: ";
//    wcin >> buffer;
//    name.assign(buffer);
//
//    wcout << "Class: ";
//    wcin >> buffer;
//    clazz.assign(buffer);
//
//    wcout << "PhoneNumber: ";
//    wcin >> buffer;
//    phoneNum.assign(buffer);
//
//    Student condition(id, name, clazz, true, phoneNum);
//
//    // search students
//    MyLinkedList<Student> searchResult = studentList.Search_Fuzzy(condition, false);
//
//    if (searchResult.size() != 0) {
//        wcout << "Here's what you got:" << endl;
//
//        for (auto iter = searchResult.begin(); iter != searchResult.end(); ++iter) {
//            Student s = *iter;
//            wcout << "ID:" << s.GetID().c_str() << endl;
//            wcout << "Name: " << s.GetName().c_str() << endl;
//            wcout << "Class: " << s.GetClass().c_str() << endl;
//            wcout << "PhoneNumber: " << s.GetPhoneNum().c_str() << endl;
//            wcout << endl;
//        }
//    } else {
//        wcout << "All students are not matches your condition." << endl;
//    }
//
//    wcout << "==Search End==" << endl;
//}

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
    ChoiceDlg confirm_dlg(console, _T("是否保存？"), _T("是否将当前系统信息保存到文件？"), 40, _T("（Y）保存"), _T("（N）舍弃"));
    if (confirm_dlg.Show() == RET_OK) {
        //SaveToFile();
    }
}

