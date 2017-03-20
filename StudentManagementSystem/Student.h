#pragma once
#include <string>
#include <ostream>
#include <sstream>
#include <TCHAR.h>

using namespace std;

class Student
{
    private:
        // 学号、姓名、班级（如：15级软件1班）、性别、手机号码、选修课程等信息。
        std::wstring id;
        std::wstring name;
        std::wstring clazz;
        bool isMale;
        std::wstring phoneNum;

    public:
        Student();
        ~Student();
        int Compare_ID(Student& other);

        Student(const std::wstring& id, const std::wstring& name, const std::wstring& clazz, bool is_male, const std::wstring& phone_num)
            : id(id),
              name(name),
              clazz(clazz),
              isMale(is_male),
              phoneNum(phone_num)
        {
        }

        std::wstring GetID() const
        {
            return id;
        }

        void SetID(const std::wstring& id)
        {
            this->id = id;
        }

        std::wstring GetName() const
        {
            return name;
        }

        void SetName(const std::wstring& name)
        {
            this->name = name;
        }

        std::wstring GetClass() const
        {
            return clazz;
        }

        void SetClass(const std::wstring& clazz)
        {
            this->clazz = clazz;
        }

        bool IsMale() const
        {
            return isMale;
        }

        void SetIsMale(const bool is_male)
        {
            isMale = is_male;
        }

        std::wstring GetPhoneNum() const
        {
            return phoneNum;
        }

        void SetPhoneNum(const std::wstring& phone_num)
        {
            phoneNum = phone_num;
        }

        void GetFormatted()
        {
            if (id.empty())id = _T(" ");
            if (name.empty())name = _T(" ");
            if (clazz.empty())clazz = _T(" ");
            if (phoneNum.empty())phoneNum = _T(" ");
        }

        friend bool operator==(const Student& lhs, const Student& rhs)
        {
            return lhs.id == rhs.id
                   && lhs.name == rhs.name
                   && lhs.clazz == rhs.clazz
                   && lhs.isMale == rhs.isMale
                   && lhs.phoneNum == rhs.phoneNum;
        }

        friend bool operator!=(const Student& lhs, const Student& rhs)
        {
            return !(lhs == rhs);
        }

        friend std::wostream& operator<<(std::wostream& os, const Student& obj)
        {
            return os
                   << obj.id << ","
                   << obj.name << ","
                   << obj.clazz << ","
                   << obj.isMale << ","
                   << obj.phoneNum;
        }

        int Compare_Name(Student& other);
        int Compare_Class(Student& other);
};

