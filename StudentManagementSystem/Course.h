#pragma once
#include <string>

using namespace std;

class Course
{
private:
    wstring name;
    wstring ID;
    wstring period;
    wstring teacherName;
public:
    Course();
    ~Course();
    wstring GetName() const
    {
        return name;
    }
    void SetName(const wstring& name)
    {
        this->name = name;
    }

    wstring GetID() const
    {
        return ID;
    }

    void SetID(const wstring& id)
    {
        ID = id;
    }

    wstring GetPeriod() const
    {
        return period;
    }

    void SetPeriod(const wstring& period)
    {
        this->period = period;
    }

    wstring GetTeacherName() const
    {
        return teacherName;
    }

    void SetTeacherName(const wstring& teacher_name)
    {
        teacherName = teacher_name;
    }
};

