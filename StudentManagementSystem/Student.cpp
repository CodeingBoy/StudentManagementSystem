#include "Student.h"



Student::Student()
{
}


Student::~Student()
{
}

int Student::Compare_ID(Student &other)
{
    return id.compare(other.id);
}

int Student::Compare_Name(Student &other)
{
    return name.compare(other.name);
}

int Student::Compare_Class(Student &other)
{
    return clazz.compare(other.clazz);
}