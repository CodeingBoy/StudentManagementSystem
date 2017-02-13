#include "CourseManagementUI.h"
#include "defs.h"
#include "Student.h"

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
        RefreshList();
        console.HighlightRow(3 + curSel);
    }
}


void CourseManagementUI::OnAddCourse()
{
}

void CourseManagementUI::OnEditCourse(int curSel)
{
}

bool CourseManagementUI::OnDeleteCourse(int curSel)
{
    return true;
}

int CourseManagementUI::ProcessInput(wchar_t input, int& curSel)
{
    switch (input) {
        case 0x41: // A - Add
            OnAddCourse();
            break;
        case 0x45: // E - Edit
            OnEditCourse(curSel);
            break;
        case 0x44: // D - Delete
            OnDeleteCourse(curSel);
            break;
        case 0x4d: // M - Switch to StudentUI
            return INPUT_SWITCH;
        case VK_PRIOR: // PgUp
            break;
        case VK_NEXT: // PgDown
            break;
        case VK_ESCAPE: // Esc - Exit
            return INPUT_EXIT;
        case VK_UP: // up arrow
            if (curSel <= 0) { // cursor at list top
                break;
            }
            curSel--;
            break;
        case VK_DOWN:
            if (curSel >= 18) { // cursor at list bottom
                break;
            }
            curSel++;
            break;
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
    console.WriteConsoleLine(_T("课程数：0┃文件：未读取"), 24, NULL, 1);
    SetStatus(_T("就绪"));

    RefreshList();
}

void CourseManagementUI::RefreshList()
{
    static const short x_num = 0, x_ID = 8, x_name = 20, x_period = 30, x_teacher = 38, x_phoneNum = 52;

    console.FillArea(list_rect, _T(' '), FOREGROUND_WHITE | BACKGROUND_GREEN);
    console.WriteConsoleLine(_T("┏━━━┳━━━━━━━┳━━━━━━━━┳━━━━━━━━┳━━━━━━━┓"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_GREEN);
    console.WriteConsoleLine(_T("┃ 序号 ┃   课程编号   ┃     课程名     ┃    开课学期    ┃   任课教师   ┃"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_GREEN);

    short y = 3;
    for (auto iter = courseList.begin(); iter != courseList.end(); ++iter) {
        Course c = *iter;
        wchar_t buffer[10] = { 0 };
        _itow(y - 3 + 1, buffer, 10);
        console.WriteConsoleLine(_T("┃ " + wstring(buffer)), { x_num, y });
        console.WriteConsoleLine(_T("┃" + c.GetID()), { x_ID, y });
        console.WriteConsoleLine(_T("┃" + c.GetName()), { x_name, y });
        console.WriteConsoleLine(_T("┃" + c.GetPeriod()), { x_period, y });
        console.WriteConsoleLine(_T("┃" + c.GetTeacherName()), { x_teacher, y });

        y++;
    }
}

int CourseManagementUI::GetSelNum(int curSelRow)
{
    return 0;
}

void CourseManagementUI::SetStatus(wstring text)
{
}
