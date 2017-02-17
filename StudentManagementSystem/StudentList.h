#pragma once
#include <TCHAR.h>
#include "MyLinkedList.h"
#include "Student.h"

class StudentList : public MyLinkedList<Student>
{
public:
    MyLinkedList<int> Search_Mask(const Student condition, bool sexMatch)
    {
        const wstring EMPTY_WSTR = wstring(_T(""));

        // get all condition;
        wstring c_name = condition.GetName(),
                c_id = condition.GetID(),
                c_clazz = condition.GetClass(),
                c_phoneNum = condition.GetPhoneNum();
        bool c_isMale = condition.IsMale();

        MyLinkedList<int> result;
        for (auto iter = begin(); iter != end(); ++iter) {
            Student s = *iter;

            bool isMatch = true;

            if (c_name != EMPTY_WSTR && s.GetName() != c_name) {
                isMatch = false;
            }

            if (isMatch && c_id != EMPTY_WSTR &&
                    s.GetID() != c_id) {
                isMatch = false;
            }

            if (isMatch && c_clazz != EMPTY_WSTR &&
                    s.GetClass() != c_clazz) {
                isMatch = false;
            }

            if (isMatch && c_phoneNum != EMPTY_WSTR &&
                    s.GetPhoneNum() != c_phoneNum) {
                isMatch = false;
            }

            if (isMatch && sexMatch &&
                    s.IsMale() != c_isMale) {
                isMatch = false;
            }

            if (isMatch) {
                result.push_back(1);
            } else {
                result.push_back(0);
            }
        }
        return result;
    }

    StudentList Search_Fuzzy(const Student condition, bool sexMatch)
    {
        const wstring EMPTY_WSTR = wstring(_T(""));

        // get all condition;
        wstring c_name = condition.GetName(),
                c_id = condition.GetID(),
                c_clazz = condition.GetClass(),
                c_phoneNum = condition.GetPhoneNum();
        bool c_isMale = condition.IsMale();

        StudentList result;
        for (auto iter = begin(); iter != end(); ++iter) {
            Student s = *iter;

            bool isMatch = true;

            if (c_name != EMPTY_WSTR && s.GetName() != c_name) {
                isMatch = false;
            }

            if (isMatch && c_id != EMPTY_WSTR &&
                    s.GetID() != c_id) {
                isMatch = false;
            }

            if (isMatch && c_clazz != EMPTY_WSTR &&
                    s.GetClass() != c_clazz) {
                isMatch = false;
            }

            if (isMatch && c_phoneNum != EMPTY_WSTR &&
                    s.GetPhoneNum() != c_phoneNum) {
                isMatch = false;
            }

            if (isMatch && sexMatch &&
                    s.IsMale() != c_isMale) {
                isMatch = false;
            }

            if (isMatch) {
                result.push_back(s);
            }
        }
        return result;
    }

    StudentList Search(const Student condition)
    {
        StudentList result;
        for (auto iter = begin(); iter != end(); ++iter) {
            const Student s = *iter;
            if (s == condition) {
                result.push_back(s);
            }
        }
        return result;
    }

    int GetIndex(const Student stu)
    {
        int index = 0;
        for (auto iter = begin(); iter != end(); ++iter) {
            const Student s = *iter;
            if (s == stu) {
                return index;
            }
            index++;
        }
        return -1;
    }

    void Sort(int weighs[])
    {

    }
};
