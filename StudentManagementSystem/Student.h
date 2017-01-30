#pragma once
#include <string>

class Student
{
public:
    Student();
    ~Student();

    std::string GetID() const
    {
        return id;
    }

    void SetID(const std::string& id)
    {
        this->id = id;
    }

    std::string GetName() const
    {
        return name;
    }

    void SetName(const std::string& name)
    {
        this->name = name;
    }

    std::string GetClass() const
    {
        return clazz;
    }

    void SetClass(const std::string& clazz)
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

    std::string GetPhoneNum() const
    {
        return phoneNum;
    }

    void SetPhoneNum(const std::string& phone_num)
    {
        phoneNum = phone_num;
    }

private:
    // 学号、姓名、班级（如：15级软件1班）、性别、手机号码、选修课程等信息。
    std::string id;
    std::string name;
    std::string clazz;
    bool isMale;
    std::string phoneNum;

};

