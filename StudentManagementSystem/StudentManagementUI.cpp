#include "StudentManagementUI.h"
#include "StudentEditDlg.h"
#include "ConfirmDlg.h"
#include "ChoiceDlg.h"

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
    }
    SetStatus(_T("��Ӽ�¼�ɹ�"));
}

void StudentManagementUI::OnEditStudent(int curSel)
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("�޼�¼"), _T("��ѡ��û�м�¼����ѡ���¼�С�"), 20);
        confirm_dlg.Show();
        return;
    }
    StudentEditDlg editDlg(console, studentList.at(selNum - 1));
    if (editDlg.Show() == DIALOG_RET_OK) {
        studentList.push_back(editDlg.GetStudent());
    }
    SetStatus(_T("�༭��¼�ɹ�"));
}

bool StudentManagementUI::OnDeleteStudent(int curSel)
{
    int selNum = GetSelNum(3 + curSel);
    if (selNum == -1) {
        ConfirmDlg confirm_dlg(console, _T("�޼�¼"), _T("��ѡ��û�м�¼����ѡ���¼�С�"), 20);
        confirm_dlg.Show();
        return true;
    }
    ChoiceDlg choiceDlg(console, _T("�Ƿ�ȷ��ɾ��"), _T("ѡ�еļ�¼����ɾ�������޷��ָ����Ƿ�ɾ����"));
    if (choiceDlg.Show() == DIALOG_RET_OK) {

    }
    SetStatus(_T("ɾ����¼�ɹ�"));
    return false;
}

int StudentManagementUI::ProcessInput(wchar_t input, int& curSel)
{
    switch (input) {
        case 0x41: // A - Add
            OnAddStudent();
            break;
        case 0x45: // E - Edit
            OnEditStudent(curSel);
            break;
        case 0x44: // D - Delete
            OnDeleteStudent(curSel);
            break;
        case 0x53: // S - Search
            OnSearchStudent();
            break;
        case 0x4d: // M - Switch to CourseUI
            return INPUT_SWITCH;
            break;
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
    return 0;
}

int StudentManagementUI::Show()
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
    console.WriteConsoleLine(_T("ѧ������ϵͳ"), 0, NULL, ALIGN_CENTER);

    // show instructions
    console.SetLineAttr(22, ATTR_OPERINSTRUCTIONS);
    console.SetLineAttr(23, ATTR_OPERINSTRUCTIONS);

    console.WriteConsoleLine(_T("��������A��׷�� ��I������ ��E���༭ ��D��ɾ�� (PgUp)��һ�� (PgDown)��һ��"), { 0, 22 });
    console.WriteConsoleLine(_T("      ��S������ ��C���鿴ѧ��ѡ�� ��M������γ���Ϣ ��Esc���˳�"), { 0, 23 });

    // show status bar
    console.SetLineAttr(24, ATTR_STATUSBAR);
    console.WriteConsoleLine(_T("��¼����0���ļ���δ��ȡ"), 24, NULL, 1);
    SetStatus(_T("����"));

    RefreshList();
}

void StudentManagementUI::RefreshList()
{
    static const short x_select = 0, x_ID = 8, x_name = 20, x_sex = 30, x_clazz = 38, x_phoneNum = 52;

    console.FillArea(list_rect, _T(' '), FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("���������ש����������ש��������ש������ש������������ש�����������������"), { 0, 1 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
    console.WriteConsoleLine(_T("�� ��� ��   ѧ��   ��  ����  �� �Ա� ��    �༶    ��    ��ϵ��ʽ    ��"), { 0, 2 }, FOREGROUND_WHITE | BACKGROUND_BLUE);

    short y = 3;
    for (auto iter = studentList.begin(); iter != studentList.end(); ++iter) {
        Student s = *iter;
        wchar_t buffer[10] = { 0 };
        _itow(y - 3 + 1, buffer, 10);
        console.WriteConsoleLine(_T("�� " + wstring(buffer)), { x_select, y });
        console.WriteConsoleLine(_T("��" + s.GetID()), { x_ID, y });
        console.WriteConsoleLine(_T("��" + s.GetName()), { x_name, y });
        console.WriteConsoleLine(s.IsMale() ? _T("��  ��") : _T("��  Ů"), { x_sex, y });
        console.WriteConsoleLine(_T("��" + s.GetClass()), { x_clazz, y });
        console.WriteConsoleLine(_T("��" + s.GetPhoneNum()), { x_phoneNum, y });

        y++;
        //console.WriteConsoleLine(output, { 0, 3 }, FOREGROUND_WHITE | BACKGROUND_BLUE);
        /*wcout << "ID:" << s.GetID().c_str() << endl;
        wcout << "Name: " << s.GetName().c_str() << endl;
        wcout << "Class: " << s.GetClass().c_str() << endl;
        wcout << "PhoneNumber: " << s.GetPhoneNum().c_str() << endl;
        wcout << endl;*/
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
    console.WriteConsoleLine(text, 24, NULL, 0);
}