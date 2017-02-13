#pragma once
#include "MyLinkedList.h"
#include <windows.h>
#include "ConsoleOperator.h"
#include "StudentList.h"

#define ATTR_OPERINSTRUCTIONS   FOREGROUND_WHITE | BACKGROUND_RED
#define ATTR_STATUSBAR BACKGROUND_INTENSITY

#define INPUT_OK 0
#define INPUT_EXIT 1

class StudentManagementUI
{
private:
    SMALL_RECT list_rect;
    StudentList studentList;
    ConsoleOperator& console;
    int curSel = 0;
public:
    StudentManagementUI(ConsoleOperator& console);
    ~StudentManagementUI();
    void OnAddStudent();
    void OnEditStudent(int curSel);
    bool OnDeleteStudent(int curSel);
    int ProcessInput(wchar_t input, int& curSel);
    int Show();
    void OnSearchStudent();
    void Draw();
    void RefreshList();
    int GetSelNum(int curSelRow);
    void SetStatus(wstring text);
};

