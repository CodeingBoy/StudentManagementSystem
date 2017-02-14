#pragma once
#include "MyLinkedList.h"
#include <windows.h>
#include "ConsoleOperator.h"
#include "StudentList.h"

#define ATTR_OPERINSTRUCTIONS   FOREGROUND_WHITE | BACKGROUND_RED
#define ATTR_STATUSBAR BACKGROUND_INTENSITY

#define INPUT_OK 0
#define INPUT_EXIT 1
#define INPUT_SWITCH 2

#define LIST_ROW_PER_PAGE 19

class StudentManagementUI
{
private:
    SMALL_RECT list_rect;
    StudentList studentList;
    ConsoleOperator& console;
    int curSel = 0;
    int curPage = 1;
    int totalPage = 1;

    void OnAddStudent();
    void OnEditStudent(int curSel);
    void OnDeleteStudent(int curSel);
    int ProcessInput(WORD input, int& curSel);
    void OnSearchStudent();
    void Draw();
    void RefreshList(int begin, int end = -1);
    int GetSelNum(int curSelRow);
    void SetStatus(wstring text);
    void CalcTotalPage();
public:
    StudentManagementUI(ConsoleOperator& console);
    ~StudentManagementUI();
    int Show();
};

