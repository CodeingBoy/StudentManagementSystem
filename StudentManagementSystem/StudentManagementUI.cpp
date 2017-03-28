#include "StudentManagementUI.h"
#include "StudentEditDlg.h"
#include "ConfirmDlg.h"
#include "ChoiceDlg.h"
#include "defs.h"
#include "StudentSearchDlg.h"
#include "InputDialog.h"
#include "StudentSortDlg.h"
#include "ChooseCourseUI.h"
#include "Main.h"

StudentManagementUI::StudentManagementUI(ConsoleOperator& console): console(console)
{
}

StudentManagementUI::~StudentManagementUI()
{
}

void StudentManagementUI::OnAddStudent()
{
    StudentEditDlg editDlg(console);
    if (editDlg.Show() == DIALOG_RET_OK) {
        studentList.push_back(editDlg.GetStudent());
        SetStatus(_T("添加记录成功"));
    }
}

void StudentManagementUI::OnEditStudent(int curSel)
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("无记录"), _T("所选行没有记录，请选择记录行。"), 20);
        confirm_dlg.Show();
        return;
    }
    StudentEditDlg editDlg(console, studentList.at(selNum - 1));
    if (editDlg.Show() == DIALOG_RET_OK) {
        auto nextiter = studentList.erase(studentList.getIter(selNum - 1));
        studentList.insert(nextiter, editDlg.GetStudent());
    }
    SetStatus(_T("编辑记录成功"));
}

bool StudentManagementUI::isInFilterMode()
{
    return mask.size() > 0;
}

void StudentManagementUI::OnDeleteStudent(int curSel)
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("无记录"), _T("所选行没有记录，请选择记录行。"), 20);
        confirm_dlg.Show();
        return;
    }
    ChoiceDlg choiceDlg(console, _T("是否确认删除"), _T("选中的记录将被删除，且无法恢复。是否删除？"));
    if (choiceDlg.Show() == DIALOG_RET_YES) {
        if (isInFilterMode()) {
            mask.erase(mask.getIter(selNum - 1));
        }
        studentList.erase(studentList.getIter(selNum - 1));
        SetStatus(_T("删除记录成功"));
    }
}

void StudentManagementUI::OnInsertStudent()
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        selNum = studentList.size();
    }
    StudentEditDlg editDlg(console);
    if (editDlg.Show() == DIALOG_RET_OK) {
        auto iter = studentList.getIter(selNum - 1);
        studentList.insert(iter, editDlg.GetStudent());

        wstringstream wss;
        wss << _T("插入记录到序号") << selNum << _T("成功");
        SetStatus(wss.str());
    }
}

void StudentManagementUI::OnSortStudent()
{
    StudentSortDlg sortDlg(console);
    if (sortDlg.Show() == DIALOG_RET_OK) {
        int weighs[5];
        sortDlg.GetWeight(weighs);
        studentList.Sort(weighs);
        SetStatus(_T("排序完毕"));
    }
}

void StudentManagementUI::OnChooseCourse()
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("无记录"), _T("所选行没有记录，请选择记录行。"), 20);
        confirm_dlg.Show();
        return;
    }

    Student stu = studentList.at(selNum - 1);
    ChooseCourseUI courseUI(console, GetCourse(), stu.GetChosenCourses());
    if (courseUI.Show() == UI_RET_EXIT) {
        stu.SetChosenCourses(courseUI.GetChosenCourse());
        studentList.Replace(selNum - 1, stu);
    }

    Draw(); // need a full refresh
}

int StudentManagementUI::ProcessInput(WORD input, int& listCurSel)
{
    switch (input) {
        case 0x41: // A - Add
            OnAddStudent();
            CalcTotalPage();
            break;
        case 0x43: // C - Choose
            OnChooseCourse();
            break;
        case 0x49: // I - Insert
            OnInsertStudent();
            CalcTotalPage();
            break;
        case 0x45: // E - Edit
            OnEditStudent(listCurSel);
            break;
        case 0x44: // D - Delete
            OnDeleteStudent(listCurSel);
            CalcTotalPage();
            break;
        case 0x46: // F - Filter
            OnSearchStudent();
            break;
        case 0x53: // S - Sort
            OnSortStudent();
            break;
        case 0x4d: // M - Switch to CourseUI
            return INPUT_SWITCH;
            break;
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
                return INPUT_NEEDNOTREFRESH;
            }
            break;
        case VK_ESCAPE: // Esc - Exit
            return INPUT_EXIT;
        case VK_UP: // up arrow
            if (listCurSel <= 0) { // cursor at list top
                if (curPage > 1) {
                    curPage--;
                    listCurSel = 18;
                    return INPUT_OK;
                } else {
                    return INPUT_NEEDNOTREFRESH;;
                }
            }
            console.SetLineAttr(listCurSel + 3, FOREGROUND_WHITE | BACKGROUND_BLUE);
            listCurSel--;
            return INPUT_NEEDNOTREFRESH;
        case VK_DOWN:
            if (listCurSel >= 18) { // cursor at list bottom
                if (curPage < totalPage) {
                    curPage++;
                    listCurSel = 0;
                    return INPUT_OK;
                } else {
                    return INPUT_NEEDNOTREFRESH;;
                }
            }
            console.SetLineAttr(listCurSel + 3, FOREGROUND_WHITE | BACKGROUND_BLUE);
            listCurSel++;
            return INPUT_NEEDNOTREFRESH;
        default:
            wstringstream buf;
            buf << _T("输入有误，不存在对应虚拟按键码 ") << input << _T(" 的动作");
            SetStatus(buf.str());
            return INPUT_NEEDNOTREFRESH;
            break;
    }
    return 0;
}

int StudentManagementUI::Show()
{
    Draw();

    // get input
    WORD input;
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

void StudentManagementUI::OnSearchStudent()
{
    StudentSearchDlg searchDlg(console);
    if (searchDlg.Show() == DIALOG_RET_OK && !searchDlg.IsAllEmpty()) {
        Student searchCondition = searchDlg.GetStudent();
        mask = studentList.Search_Mask(searchCondition, searchDlg.GetSearchSex());

        // calc filtered count
        filteredCount = 0;
        for(auto iter = mask.begin(); iter != mask.end(); ++iter) {
            if (*iter == 1) {
                filteredCount++;
            }
        }

        CalcTotalPage();
        if(curPage > totalPage) {
            curPage = totalPage; // filp to last page
        }

        RefreshList(0);
        SetStatus(_T("筛选记录完毕"));
    } else {
        mask.clear();
        filteredCount = 0;
        CalcTotalPage();
        RefreshList(0);
        SetStatus(_T("退出筛选模式"));
    }
}

void StudentManagementUI::Draw()
{
    console.HideCursor(); // hide cursor at first
    COORD size = console.GetSize();
    console.ClearScreen(FOREGROUND_WHITE | BACKGROUND_BLUE); // set background

    // set list rect
    list_rect = { 0, 1, size.X - 1, 21 };

    // show title
    console.SetLineAttr(0, FOREGROUND_WHITE | BACKGROUND_RED);
    console.WriteConsoleLine(_T("学生管理系统"), 0, NULL, ALIGN_CENTER);

    // show instructions
    console.SetLineAttr(22, ATTR_OPERINSTRUCTIONS);
    console.SetLineAttr(23, ATTR_OPERINSTRUCTIONS);

    console.WriteConsoleLine(_T("操作：（A）追加 （I）插入 （E）编辑 （D）删除 (PgUp)上一屏 (PgDown)下一屏"), { 0, 22 });
    console.WriteConsoleLine(_T("      （F）筛选 （S）排序 （C）查看学生选课 （M）管理课程信息 （Esc）退出"), { 0, 23 });

    // show status bar
    console.SetLineAttr(24, ATTR_STATUSBAR);

    RefreshStatusInf();
    SetStatus(_T("就绪"));

    CalcTotalPage();
    RefreshList(0);
}

void StudentManagementUI::RefreshList(int begin, int pageLength)
{
    StudentList& showingList = *pShowingList;

    static const short x_select = 0, x_ID = 8, x_name = 24, x_sex = 38, x_clazz = 46, x_phoneNum = 62, x_end = 78;

    console.FillArea(list_rect, _T(' '), FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("┏━━━┳━━━━━━━┳━━━━━━┳━━━┳━━━━━━━┳━━━━━━━┓"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("┃ 序号 ┃     学号     ┃    姓名    ┃ 性别 ┃     班级     ┃   联系方式   ┃"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_BLUE);

    auto maskIter = mask.begin();
    if (mask.size() > 0) {
        int skipCount = 0;
        int totalCount = 0;
        while (skipCount < begin && maskIter != mask.end()) {
            int maskCode = *maskIter;
            if (maskCode == 1) {
                skipCount++;
            }
            totalCount++;
            ++maskIter;
        }
        begin = totalCount;
    }

    int index = 0;

    short y = 3;
    auto iter = showingList.getIter(begin);
    int recordShownCount = 0;
    while (recordShownCount < pageLength && iter != studentList.end()) {
        Student s = *iter;

        if (mask.size() > 0) {// mask is vaild
            if(maskIter == mask.end()) {
                break;
            }
            int maskCode = *maskIter;
            ++maskIter;
            if (maskCode != 1) {
                index++;
                ++iter;
                continue;
            }
        }

        wchar_t buffer[10] = { 0 };
        _itow(index + begin + 1, buffer, 10);
        console.WriteConsoleLine(_T("┃ " + wstring(buffer)), { x_select, y });
        console.WriteConsoleLine(_T("┃" + s.GetID()), { x_ID, y });
        console.WriteConsoleLine(_T("┃" + s.GetName()), { x_name, y });
        console.WriteConsoleLine(s.IsMale() ? _T("┃  男") : _T("┃  女"), { x_sex, y });
        console.WriteConsoleLine(_T("┃" + s.GetClass()), { x_clazz, y });
        console.WriteConsoleLine(_T("┃" + s.GetPhoneNum()), { x_phoneNum, y });
        console.WriteConsoleLine(_T("┃"), { x_end, y });
        y++;

        index++;
        recordShownCount++;
        ++iter;
    }
}

int StudentManagementUI::GetSelNum(int curSelRow)
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

Student StudentManagementUI::GetSelStudent(int curSelRow)
{
    SMALL_RECT IDarea_rect = { 10, curSelRow, 24, curSelRow },
               namearea_rect = { 26, curSelRow, 38, curSelRow },
               sexarea_rect = { 42, curSelRow, 46, curSelRow },
               classarea_rect = { 48, curSelRow, 62, curSelRow },
               phonenumarea_rect = { 64, curSelRow, 78, curSelRow };
    const int col_count = 14;
    CHAR_INFO info[col_count * 5];
    ReadConsoleOutput(console.GetConsoleHandle(), info, { col_count, 5 }, { 0, 0 }, &IDarea_rect);
    ReadConsoleOutput(console.GetConsoleHandle(), info, { col_count, 5 }, { 0, 1 }, &namearea_rect);
    ReadConsoleOutput(console.GetConsoleHandle(), info, { col_count, 5 }, { 0, 2 }, &sexarea_rect);
    ReadConsoleOutput(console.GetConsoleHandle(), info, { col_count, 5 }, { 0, 3 }, &classarea_rect);
    ReadConsoleOutput(console.GetConsoleHandle(), info, { col_count, 5 }, { 0, 4 }, &phonenumarea_rect);

    wstring ID = InputDialog::ParseCharInfos(info, 0 * col_count, 1 * col_count - 1);
    wstring name = InputDialog::ParseCharInfos(info, 1 * col_count, 2 * col_count - 1);
    wstring sex = InputDialog::ParseCharInfos(info, 2 * col_count, 3 * col_count - 1);
    wstring clazz = InputDialog::ParseCharInfos(info, 3 * col_count, 4 * col_count - 1);
    wstring phoneNum = InputDialog::ParseCharInfos(info, 4 * col_count, 5 * col_count - 1);

    return Student(ID, name, clazz, sex.compare(_T("男")), phoneNum);
}

void StudentManagementUI::SetStatus(wstring text)
{
    console.FillAreaChar({ 0, 24, 50, 24 }, _T(' '));
    console.WriteConsoleLine(text, 24, NULL, 0);
}

void StudentManagementUI::CalcTotalPage()
{
    if (mask.size() > 0) {
        totalPage = ceil(filteredCount / static_cast<double>(LIST_ROW_PER_PAGE));
    } else {
        totalPage = ceil(studentList.size() / static_cast<double>(LIST_ROW_PER_PAGE));
    }
    if (totalPage < 1)totalPage = 1; // at least 1 page
}

void StudentManagementUI::RefreshStatusInf()
{
    wstringstream wss;
    wss << curPage << _T("/") << totalPage << _T("页");
    wss << _T("┃");
    wss << _T("记录数：") << studentList.size();

    console.WriteConsoleLine(wss.str(), 24, NULL, 1);
}
