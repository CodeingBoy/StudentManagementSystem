#include "StudentManagementUI.h"
#include "StudentEditDlg.h"
#include "ConfirmDlg.h"
#include "ChoiceDlg.h"
#include "defs.h"

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

int StudentManagementUI::ProcessInput(WORD input, int& listCurSel)
{
    switch (input) {
        case 0x41: // A - Add
            OnAddStudent();
            CalcTotalPage();
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
        case 0x53: // S - Search
            OnSearchStudent();
            break;
        case 0x4d: // M - Switch to CourseUI
            return INPUT_SWITCH;
            break;
        case VK_PRIOR: // PgUp
            if (curPage > 1)curPage--;
            else {
                SetStatus(_T("没有上一页了"));
            }
            break;
        case VK_NEXT: // PgDown
            if (curPage < totalPage)curPage++;
            else {
                SetStatus(_T("没有下一页了"));
            }
            break;
        case VK_ESCAPE: // Esc - Exit
            return INPUT_EXIT;
        case VK_UP: // up arrow
            if (listCurSel <= 0) { // cursor at list top
                if (curPage > 1) {
                    curPage--;
                    listCurSel = 18;
                }
                break;
            }
            listCurSel--;
            break;
        case VK_DOWN:
            if (listCurSel >= 18) { // cursor at list bottom
                if (curPage < totalPage) {
                    curPage++;
                    listCurSel = 0;
                }
                break;
            }
            listCurSel++;
            break;
        default:
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
        RefreshList(LIST_ROW_PER_PAGE * (curPage - 1));
        RefreshStatusInf();
        console.HighlightRow(3 + curSel);
    }
}

void StudentManagementUI::OnSearchStudent()
{
}

void StudentManagementUI::Draw()
{
    console.HideCursor(); // hide cursor at first
    COORD size = console.GetSize();
    console.SetSize({ size.X, 25 });
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
    console.WriteConsoleLine(_T("      （S）搜索 （C）查看学生选课 （M）管理课程信息 （Esc）退出"), { 0, 23 });

    // show status bar
    console.SetLineAttr(24, ATTR_STATUSBAR);

    RefreshStatusInf();
    SetStatus(_T("就绪"));

    RefreshList(0);
}

void StudentManagementUI::RefreshList(int begin, int end)
{
    if (end == -1)end = begin + LIST_ROW_PER_PAGE - 1;
    if (end > studentList.size())end = studentList.size();

    static const short x_select = 0, x_ID = 8, x_name = 24, x_sex = 38, x_clazz = 46, x_phoneNum = 62, x_end = 78;

    console.FillArea(list_rect, _T(' '), FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("┏━━━┳━━━━━━━┳━━━━━━┳━━━┳━━━━━━━┳━━━━━━━┓"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("┃ 序号 ┃     学号     ┃    姓名    ┃ 性别 ┃     班级     ┃   联系方式   ┃"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_BLUE);

    short y = 3;
    for (auto iter = studentList.getIter(begin); iter != studentList.getIter(end + 1); ++iter) {
        Student s = *iter;
        wchar_t buffer[10] = { 0 };
        _itow(y - 3 + 1 + begin, buffer, 10);
        console.WriteConsoleLine(_T("┃ " + wstring(buffer)), { x_select, y });
        console.WriteConsoleLine(_T("┃" + s.GetID()), { x_ID, y });
        console.WriteConsoleLine(_T("┃" + s.GetName()), { x_name, y });
        console.WriteConsoleLine(s.IsMale() ? _T("┃  男") : _T("┃  女"), { x_sex, y });
        console.WriteConsoleLine(_T("┃" + s.GetClass()), { x_clazz, y });
        console.WriteConsoleLine(_T("┃" + s.GetPhoneNum()), { x_phoneNum, y });
        console.WriteConsoleLine(_T("┃"), { x_end, y });

        y++;
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

void StudentManagementUI::SetStatus(wstring text)
{
    console.FillAreaChar({ 0, 24, 50, 24 }, _T(' '));
    console.WriteConsoleLine(text, 24, NULL, 0);
}

void StudentManagementUI::CalcTotalPage()
{
    totalPage = ceil(studentList.size() / static_cast<double>(LIST_ROW_PER_PAGE));
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