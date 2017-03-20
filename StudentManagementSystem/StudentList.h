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

        void Replace(int index, Student stu)
        {
            auto nextiter = erase(getIter(index));
            insert(nextiter, stu);
        }

        mylklist_iterator Replace(mylklist_iterator iter, Student stu)
        {
            auto nextiter = erase(iter);
            insert(nextiter, stu);
            return nextiter;
        }


        void SortByID()
        {
            QuickSort_ID(0, size());
        }

        void SortByName()
        {
            QuickSort_Name(0, size());
        }

        void SortBySex()
        {
            mylklist_iterator iter = begin();
            mylklist_reverse_iterator riter = rbegin();

            int index = 0, index_reverse = size() - 1;

            while (index < index_reverse) {
                while ((*iter).IsMale() && index < index_reverse) {
                    ++iter;
                    index++;
                }

                while (!(*riter).IsMale() && index < index_reverse) {
                    ++riter;
                    index_reverse--;
                }

                if (index < index_reverse) {
                    Student a = *iter; // copy first
                    Student b = *riter;

                    // swap
                    iter = Replace(iter, b); // replace a with b
                    riter = Replace(riter, a);
                }
            }
        }

        void SortByClass()
        {

        }

        void SortByPhoneNum()
        {

        }

        void Sort(int weighs[])
        {
            while (weighs[0] > 0 || weighs[1] > 0 || weighs[2] > 0 || weighs[3] > 0 || weighs[4] > 0) {
                if (weighs[0] == 1) {
                    SortByID();
                }

                if (weighs[1] == 1) {
                    SortByName();
                }

                if (weighs[2] == 1) {
                    SortBySex();
                }

                if (weighs[3] == 1) {
                    SortByClass();
                }

                if (weighs[4] == 1) {
                    SortByPhoneNum();
                }

                weighs[0]--;
                weighs[1]--;
                weighs[2]--;
                weighs[3]--;
                weighs[4]--;
            }
        }

        void QuickSort_ID(const int start, const int end)
        {
            if (start >= end)return;

            Student pivot = at(start);

            int index_left = start, index_right = end - 1;
            while (index_left < index_right) {
                while (index_left < index_right && pivot.Compare_ID(at(index_right)) <= 0)index_right--;
                while (index_left < index_right && pivot.Compare_ID(at(index_left)) >= 0)index_left++;

                if (index_left < index_right) {
                    mylklist_iterator iter_left = getIter(index_left);
                    mylklist_iterator iter_right = getIter(index_right);

                    Swap(iter_left, iter_right);
                }
            }

            if (start != index_left)
                Swap(getIter(start), getIter(index_left));

            QuickSort_ID(start, index_left);
            QuickSort_ID(index_left + 1, end);
        }

        void QuickSort_Name(const int start, const int end)
        {
            if (start >= end)return;

            Student pivot = at(start);

            int index_left = start, index_right = end - 1;
            while(index_left < index_right) {
                while (index_left < index_right && pivot.Compare_Name(at(index_right)) <= 0)index_right--;
                while (index_left < index_right && pivot.Compare_Name(at(index_left)) >= 0)index_left++;

                if(index_left < index_right) {
                    mylklist_iterator iter_left = getIter(index_left);
                    mylklist_iterator iter_right = getIter(index_right);

                    Swap(iter_left, iter_right);
                }
            }

            if(start != index_left)
                Swap(getIter(start), getIter(index_left));

            QuickSort_Name(start, index_left);
            QuickSort_Name(index_left + 1, end);
        }

        void Swap(mylklist_iterator a, mylklist_iterator b)
        {
            Student temp = *a;
            Replace(a, *b);
            Replace(b, temp);
        }
};