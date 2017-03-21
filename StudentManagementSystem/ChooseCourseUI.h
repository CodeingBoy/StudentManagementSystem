#pragma once
#include <windows.h>
#include "MyLinkedList.h"
#include "Course.h"
#include "ConsoleOperator.h"

#define ATTR_OPERINSTRUCTIONS   FOREGROUND_WHITE | BACKGROUND_RED
#define ATTR_STATUSBAR BACKGROUND_INTENSITY
#define ATTR_CHOSEN FOREGROUND_RED | BACKGROUND_INTENSITY

#define INPUT_OK 0
#define INPUT_EXIT 1
#define INPUT_SWITCH 2
#define INPUT_NEEDNOTREFRESH 3

#define LIST_ROW_PER_PAGE 19

class ChooseCourseUI
{
    SMALL_RECT list_rect;
    ConsoleOperator& console;
    MyLinkedList<Course> courses_list;
    int curSel = 0;
    int curPage = 1;
    int totalPage = 1;
    int chosenCount = 0;
    bool *chosen;
public:
    ChooseCourseUI(ConsoleOperator& console, MyLinkedList<Course> courses_list, MyLinkedList<wstring> chosen_list);
    ~ChooseCourseUI();
    void OnChoose();
    int ProcessInput(wchar_t input, int& curSel);
    void Draw();
    void RefreshList(int begin, int pageLength = LIST_ROW_PER_PAGE);
    void RefreshStatusInf();
    int GetSelNum(int curSelRow);
    void SetStatus(wstring text);
    void CalcTotalPage();
    MyLinkedList<wstring> GetChosenCourse();
    int Show();
};

