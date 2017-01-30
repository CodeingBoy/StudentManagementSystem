#pragma once
#include <string>

class Student
{
public:
    Student();
    ~Student();

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

private:
    // 学号、姓名、班级（如：15级软件1班）、性别、手机号码、选修课程等信息。
    std::wstring id;
    std::wstring name;
    std::wstring clazz;
    bool isMale;
    std::wstring phoneNum;

};

