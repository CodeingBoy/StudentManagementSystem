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
#define INPUT_NEEDNOTREFRESH 3

#define LIST_ROW_PER_PAGE 19

class StudentManagementUI
{
private:
    SMALL_RECT list_rect;
    StudentList studentList, searchList;
    ConsoleOperator& console;
    int curSel = 0;
    int curPage = 1;
    int totalPage = 1;
    StudentList *pShowingList = &studentList;
    MyLinkedList<int> mask;

    void OnAddStudent();
    void OnEditStudent(int curSel);
    bool isInFilterMode();
    void OnDeleteStudent(int curSel);
    void OnInsertStudent();
    void OnSortStudent();
    void OnChooseCourse();
    int ProcessInput(WORD input, int& curSel);
    void OnSearchStudent();
    void Draw();
    void RefreshList(int begin, int pageLength = LIST_ROW_PER_PAGE);
    int GetSelNum(int curSelRow);
    Student GetSelStudent(int curSelRow);
    void SetStatus(wstring text);
    void CalcTotalPage();
    void RefreshStatusInf();
public:
    StudentManagementUI(ConsoleOperator& console);
    ~StudentManagementUI();
    int Show();

    StudentList& GetStudentList()
    {
        return studentList;
    }

};

