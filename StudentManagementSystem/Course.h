#pragma once
#include <string>

class Course
{
public:
    Course();
    ~Course();
private:
    std::string name;

public:
    std::string GetName() const
    {
        return name;
    }
};

