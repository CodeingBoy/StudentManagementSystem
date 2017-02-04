#include <cstdio>
#include <iostream>
#include <TCHAR.h>
#include "Student.h"
#include "LinkedListSel.h"
#include "FileHandler.h"

using namespace std;

StudentList studentList;

void AddStudent();
void ShowStudent();
void SearchStudent();

int main()
{
    int select = 0;
    do {
        cout << "Welcome using Student Management System\n" << endl;
        printf("Please sel func:\n");
        printf("1. add\n");
        printf("2. del\n");
        printf("3. show\n");
        printf("4. search\n");

        wcin >> select;

        switch (select) {
            case 1:
                AddStudent();
                break;
            case 3:
                ShowStudent();
                break;
            case 4:
                SearchStudent();
                break;
            default:
                break;
        }

    } while (select);

    cin.get();
}

void AddStudent()
{
    wstring id, name, clazz, phoneNum;

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

    // ask for confirm
    wcout << endl;
    wcout << "Here's what you have entered: " << endl;
    wcout << "ID:" << id.c_str() << endl;
    wcout << "Name: " << name.c_str() << endl;
    wcout << "Class: " << clazz.c_str() << endl;
    wcout << "PhoneNumber: " << phoneNum.c_str() << endl;
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

void ShowStudent()
{
    for (auto iter = studentList.begin(); iter != studentList.end(); ++iter) {
        Student s = *iter;
        wcout << "ID:" << s.GetID().c_str() << endl;
        wcout << "Name: " << s.GetName().c_str() << endl;
        wcout << "Class: " << s.GetClass().c_str() << endl;
        wcout << "PhoneNumber: " << s.GetPhoneNum().c_str() << endl;
        wcout << endl;
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