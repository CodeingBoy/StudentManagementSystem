#pragma once
#include <windows.h>
#include "MyLinkedList.h"
#include "Course.h"
#include "ConsoleOperator.h"

#define ATTR_OPERINSTRUCTIONS   FOREGROUND_WHITE | BACKGROUND_RED
#define ATTR_STATUSBAR BACKGROUND_INTENSITY

#define INPUT_OK 0
#define INPUT_EXIT 1
#define INPUT_SWITCH 2

class CourseManagementUI
{
private:
    SMALL_RECT list_rect;
    MyLinkedList<Course> courseList;
    ConsoleOperator& console;
    int curSel = 0;
    void OnAddCourse();
    void OnEditCourse(int curSel);
    bool OnDeleteCourse(int curSel);
    int ProcessInput(wchar_t input, int& curSel);
    void Draw();
    void RefreshList();
    int GetSelNum(int curSelRow);
    void SetStatus(wstring text);
public:
    CourseManagementUI(ConsoleOperator& console);
    ~CourseManagementUI();
    int Show();
};

