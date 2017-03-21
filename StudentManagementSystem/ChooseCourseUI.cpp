#include "ChooseCourseUI.h"
#include "defs.h"
#include "Student.h"
#include "Dialog.h"
#include "CourseEditDlg.h"
#include <filesystem>
#include "ConfirmDlg.h"

ChooseCourseUI::ChooseCourseUI(ConsoleOperator& console, MyLinkedList<Course> courses_list, MyLinkedList<wstring> chosen_list)
    : console(console), courses_list(courses_list)
{
    chosen = new bool[courses_list.size()];
    memset(chosen, 0, courses_list.size() * sizeof(bool));

    if(chosen_list.size() != 0) {
        int index = 0;
        for (auto iter = courses_list.begin(); iter != courses_list.end(); ++iter) {
            MyLinkedList<wstring>::mylklist_iterator search_iter = chosen_list.search((*iter).GetID());
            if(search_iter.GetNodePtr() != nullptr) {
                chosen[index] = true;
                chosenCount++;
            }
            index++;
        }
    }
}

ChooseCourseUI::~ChooseCourseUI()
{
    delete[] chosen;
}

void ChooseCourseUI::OnChoose()
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("�޼�¼"), _T("��ѡ��û�пγ̣���ѡ���¼�С�"), 20);
        confirm_dlg.Show();
        return;
    }
    chosen[selNum - 1] = !chosen[selNum - 1];
    if(chosen[selNum - 1]) {
        chosenCount++;
    } else {
        chosenCount--;
    }

    SetStatus(_T("ѡ��γ̳ɹ�"));
}

int ChooseCourseUI::Show()
{
    Draw();

    // get input
    wchar_t input;
    while ((input = console.GetPressedDownKey())) {
        int retcode = ProcessInput(input, curSel);
        switch (retcode) {
            case INPUT_EXIT:
                return UI_RET_EXIT;
        }
        console.HideCursor();
        //if (retcode != INPUT_NEEDNOTREFRESH) {
        RefreshList(LIST_ROW_PER_PAGE * (curPage - 1));
        RefreshStatusInf();
        //}
        console.HighlightRow(3 + curSel);
    }
}

int ChooseCourseUI::ProcessInput(wchar_t input, int& curSel)
{
    switch (input) {
        case VK_SPACE: // Choose
            OnChoose();
            break;
        case VK_PRIOR: // PgUp
            if (curPage > 1)curPage--;
            else {
                SetStatus(_T("û����һҳ��"));
                return INPUT_NEEDNOTREFRESH;;
            }
            break;
        case VK_NEXT: // PgDown
            if (curPage < totalPage)curPage++;
            else {
                SetStatus(_T("û����һҳ��"));
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
    return INPUT_OK;
}

void ChooseCourseUI::Draw()
{
    console.HideCursor(); // hide cursor at first
    COORD size = console.GetSize();
    console.SetSize({ size.X, 25 });
    console.ClearScreen(FOREGROUND_WHITE | BACKGROUND_GREEN); // set background

    // set list rect
    list_rect = { 0, 1, size.X - 1, 21 };

    // show title
    console.SetLineAttr(0, FOREGROUND_WHITE | BACKGROUND_RED);
    console.WriteConsoleLine(_T("ѧ������ϵͳ - �γ�ѡ��"), 0, NULL, ALIGN_CENTER);

    // show instructions
    console.SetLineAttr(22, ATTR_OPERINSTRUCTIONS);
    console.SetLineAttr(23, ATTR_OPERINSTRUCTIONS);

    console.WriteConsoleLine(_T("������( )ѡ�� (PgUp)��һ�� (PgDown)��һ�� ��Esc���˳�"), { 0, 22 });
    console.WriteConsoleLine(_T("       "), { 0, 23 });

    // show status bar
    console.SetLineAttr(24, ATTR_STATUSBAR);
    RefreshStatusInf();
    SetStatus(_T("����"));

    CalcTotalPage();
    RefreshList(0);
}

void ChooseCourseUI::RefreshList(int begin, int pageLength)
{
    static const short x_num = 0, x_ID = 8, x_name = 20, x_period = 50, x_teacher = 68, x_end = 78;

    console.FillArea(list_rect, _T(' '), FOREGROUND_WHITE | BACKGROUND_GREEN);
    console.WriteConsoleLine(_T("���������ש����������ש����������������������������ש����������������ש���������"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_GREEN);
    console.WriteConsoleLine(_T("�� ��� �� �γ̱�� ��           �γ���           ��    ����ѧ��    ���ον�ʦ��"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_GREEN);

    int index = 0;

    short y = 3;
    auto iter = courses_list.getIter(begin);
    while (index < pageLength && iter != courses_list.end()) {
        Course c = *iter;
        wchar_t buffer[10] = { 0 };
        _itow(begin + index + 1, buffer, 10);
        console.WriteConsoleLine(_T("�� " + wstring(buffer)), { x_num, y });
        console.WriteConsoleLine(_T("��" + c.GetID()), { x_ID, y });
        console.WriteConsoleLine(_T("��" + c.GetName()), { x_name, y });
        console.WriteConsoleLine(_T("��" + c.GetPeriod()), { x_period, y });
        console.WriteConsoleLine(_T("��" + c.GetTeacherName()), { x_teacher, y });
        console.WriteConsoleLine(_T("��"), { x_end, y });

        if(chosen[begin + index]) {
            console.HighlightRow(y, ATTR_CHOSEN);
        }

        y++;
        index++;
        ++iter;
    }
}

void ChooseCourseUI::RefreshStatusInf()
{
    wstringstream wss;
    wss << curPage << _T("/") << totalPage << _T("ҳ");
    wss << _T("��");
    wss << _T("��ѡ����") << chosenCount;
    wss << _T("��");
    wss << _T("��¼����") << courses_list.size();

    console.WriteConsoleLine(wss.str(), 24, NULL, 1);
}

int ChooseCourseUI::GetSelNum(int curSelRow)
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

void ChooseCourseUI::SetStatus(wstring text)
{
    console.FillAreaChar({ 0, 24, 50, 24 }, _T(' '));
    console.WriteConsoleLine(text, 24, NULL, 0);
}

void ChooseCourseUI::CalcTotalPage()
{
    totalPage = ceil(courses_list.size() / static_cast<double>(LIST_ROW_PER_PAGE));
    if (totalPage < 1)totalPage = 1; // at least 1 page
}

MyLinkedList<wstring> ChooseCourseUI::GetChosenCourse()
{
    MyLinkedList<wstring> chosen_list;

    int index = 0;
    for (auto iter = courses_list.begin(); iter != courses_list.end(); ++iter) {
        if (chosen[index]) {
            chosen_list.push_back((*iter).GetID());
        }
        index++;
    }
    return chosen_list;
}