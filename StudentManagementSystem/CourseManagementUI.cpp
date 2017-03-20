#include "CourseManagementUI.h"
#include "defs.h"
#include "Student.h"
#include "Dialog.h"
#include "CourseEditDlg.h"
#include "ConfirmDlg.h"
#include "ChoiceDlg.h"
#include <filesystem>

CourseManagementUI::CourseManagementUI(ConsoleOperator& console): console(console)
{
}

CourseManagementUI::~CourseManagementUI()
{
}

int CourseManagementUI::Show()
{
    Draw();

    // get input
    wchar_t input;
    while ((input = console.GetPressedDownKey())) {
        int retcode = ProcessInput(input, curSel);
        switch (retcode) {
            case INPUT_EXIT:
                return UI_RET_EXIT;
            case INPUT_SWITCH:
                return UI_RET_SWITCH;
        }
        console.HideCursor();
        if (retcode != INPUT_NEEDNOTREFRESH) {
            RefreshList(LIST_ROW_PER_PAGE * (curPage - 1));
            RefreshStatusInf();
        }
        console.HighlightRow(3 + curSel);
    }
}


void CourseManagementUI::OnAddCourse()
{
    CourseEditDlg editDlg(console);
    if (editDlg.Show() == DIALOG_RET_OK) {
        courseList.push_back(editDlg.GetCourse());
        SetStatus(_T("添加记录成功"));
    }
}

void CourseManagementUI::OnEditCourse(int curSel)
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("无记录"), _T("所选行没有记录，请选择记录行。"), 20);
        confirm_dlg.Show();
        return;
    }
    CourseEditDlg editDlg(console, courseList.at(selNum - 1));
    if (editDlg.Show() == DIALOG_RET_OK) {
        auto nextiter = courseList.erase(courseList.getIter(selNum - 1));
        courseList.insert(nextiter, editDlg.GetCourse());
    }
    SetStatus(_T("编辑记录成功"));
}

void CourseManagementUI::OnDeleteCourse(int curSel)
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("无记录"), _T("所选行没有记录，请选择记录行。"), 20);
        confirm_dlg.Show();
        return;
    }
    ChoiceDlg choiceDlg(console, _T("是否确认删除"), _T("选中的记录将被删除，且无法恢复。是否删除？"));
    if (choiceDlg.Show() == DIALOG_RET_YES) {
        courseList.erase(courseList.getIter(selNum - 1));
        SetStatus(_T("删除记录成功"));
    }
}

int CourseManagementUI::ProcessInput(wchar_t input, int& curSel)
{
    switch (input) {
        case 0x41: // A - Add
            OnAddCourse();
            CalcTotalPage();
            break;
        case 0x45: // E - Edit
            OnEditCourse(curSel);
            break;
        case 0x44: // D - Delete
            OnDeleteCourse(curSel);
            CalcTotalPage();
            break;
        case 0x4d: // M - Switch to StudentUI
            return INPUT_SWITCH;
        case VK_PRIOR: // PgUp
            if (curPage > 1)curPage--;
            else {
                SetStatus(_T("没有上一页了"));
                return INPUT_NEEDNOTREFRESH;;
            }
            break;
        case VK_NEXT: // PgDown
            if (curPage < totalPage)curPage++;
            else {
                SetStatus(_T("没有下一页了"));
                return INPUT_NEEDNOTREFRESH;;
            }
            break;
        case VK_ESCAPE: // Esc - Exit
            return INPUT_EXIT;
        case VK_UP: // up arrow
            if (curSel <= 0) { // cursor at list top
                if (curPage > 1) {
                    curPage--;
                    curSel = 18;
                    return INPUT_OK;
                } else {
                    return INPUT_NEEDNOTREFRESH;;
                }
            }
            console.SetLineAttr(curSel + 3, FOREGROUND_WHITE | BACKGROUND_GREEN);
            curSel--;
            return INPUT_NEEDNOTREFRESH;
        case VK_DOWN:
            if (curSel >= 18) { // cursor at list bottom
                if (curPage < totalPage) {
                    curPage++;
                    curSel = 0;
                    return INPUT_OK;
                } else {
                    return INPUT_NEEDNOTREFRESH;;
                }
            }
            console.SetLineAttr(curSel + 3, FOREGROUND_WHITE | BACKGROUND_GREEN);
            curSel++;
            return INPUT_NEEDNOTREFRESH;
        default:
            break;
    }
}

void CourseManagementUI::Draw()
{
    console.HideCursor(); // hide cursor at first
    COORD size = console.GetSize();
    console.SetSize({ size.X, 25 });
    console.ClearScreen(FOREGROUND_WHITE | BACKGROUND_GREEN); // set background

    // set list rect
    list_rect = { 0, 1, size.X - 1, 21 };

    // show title
    console.SetLineAttr(0, FOREGROUND_WHITE | BACKGROUND_RED);
    console.WriteConsoleLine(_T("学生管理系统 - 课程管理"), 0, NULL, ALIGN_CENTER);

    // show instructions
    console.SetLineAttr(22, ATTR_OPERINSTRUCTIONS);
    console.SetLineAttr(23, ATTR_OPERINSTRUCTIONS);

    console.WriteConsoleLine(_T("操作：（A）追加 （E）编辑 （D）删除 (PgUp)上一屏 (PgDown)下一屏"), { 0, 22 });
    console.WriteConsoleLine(_T("      （M）管理学生信息 （Esc）退出"), { 0, 23 });

    // show status bar
    console.SetLineAttr(24, ATTR_STATUSBAR);
    RefreshStatusInf();
    SetStatus(_T("就绪"));

    CalcTotalPage();
    RefreshList(0);
}

void CourseManagementUI::RefreshList(int begin, int pageLength)
{
    static const short x_num = 0, x_ID = 8, x_name = 20, x_period = 50, x_teacher = 68, x_end = 78;

    console.FillArea(list_rect, _T(' '), FOREGROUND_WHITE | BACKGROUND_GREEN);
    console.WriteConsoleLine(_T("┏━━━┳━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━┳━━━━┓"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_GREEN);
    console.WriteConsoleLine(_T("┃ 序号 ┃ 课程编号 ┃           课程名           ┃    开课学期    ┃任课教师┃"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_GREEN);

    int index = 0;

    short y = 3;
    auto iter = courseList.getIter(begin);
    while (index < pageLength && iter != courseList.end()) {
        Course c = *iter;
        wchar_t buffer[10] = { 0 };
        _itow(begin + index + 1, buffer, 10);
        console.WriteConsoleLine(_T("┃ " + wstring(buffer)), { x_num, y });
        console.WriteConsoleLine(_T("┃" + c.GetID()), { x_ID, y });
        console.WriteConsoleLine(_T("┃" + c.GetName()), { x_name, y });
        console.WriteConsoleLine(_T("┃" + c.GetPeriod()), { x_period, y });
        console.WriteConsoleLine(_T("┃" + c.GetTeacherName()), { x_teacher, y });
        console.WriteConsoleLine(_T("┃"), { x_end, y });

        y++;
        index++;
        ++iter;
    }
}

void CourseManagementUI::RefreshStatusInf()
{
    wstringstream wss;
    wss << curPage << _T("/") << totalPage << _T("页");
    wss << _T("┃");
    wss << _T("记录数：") << courseList.size();

    console.WriteConsoleLine(wss.str(), 24, NULL, 1);
}

int CourseManagementUI::GetSelNum(int curSelRow)
{
    SMALL_RECT numarea_rect = { 3, curSelRow, 7, curSelRow };
    CHAR_INFO info[5];
    ReadConsoleOutput(console.GetConsoleHandle(), info, { 5, 1 }, { 0, 0 }, &numarea_rect);

    wchar_t buffer[6] = { 0 };
    for (int i = 0; i < 5; ++i) {
        wchar_t uchar = info[i].Char.UnicodeChar;
        if (uchar == _T(' '))break;
        buffer[i] = uchar;
    }
    if (!wcscmp(buffer, _T(""))) {
        return -1;
    } else {
        return _wtoi(buffer);
    }
}

void CourseManagementUI::SetStatus(wstring text)
{
    console.FillAreaChar({ 0, 24, 50, 24 }, _T(' '));
    console.WriteConsoleLine(text, 24, NULL, 0);
}

void CourseManagementUI::CalcTotalPage()
{
    totalPage = ceil(courseList.size() / static_cast<double>(LIST_ROW_PER_PAGE));
    if (totalPage < 1)totalPage = 1; // at least 1 page
}