#include "StuCSVParser.h"
#include <TCHAR.h>
#include <vector>

#define LIST_ERR -1
#define LIST_ERR_STR (_T("Error"))

CStuCSVParser::CStuCSVParser(CFileHandler *pHandler)
{
    this->fileHandler = pHandler;
}

CStuCSVParser::~CStuCSVParser()
{
}

bool CStuCSVParser::Parse(bool haveHeader, MyLinkedList<Student> *plist, const wchar_t *delimiter)
{
    if (haveHeader)
        delete fileHandler->ReadLine(); // 忽略第一行


    wchar_t line[1024];
    while (!fileHandler->isEOF()) {
        Student inf;

        memset(line, NULL, 1024); // 设置为空
        if (!fileHandler->ReadLine(line) || !wcscmp(line, _T("")))
            continue;
        if (delimiter != NULL && !wcscmp(line, delimiter))
            return true;
        int size = Parse(line, inf);
        plist->push_back(inf);
        if (size < 6)
            hasDataError = true;
        else if (size > 7)
            hasExtraInf = true;
        parsedLine++;
    }

    return true;
}

MyLinkedList<wstring> CStuCSVParser::ParseChosenCourse(wchar_t *str)
{
    MyLinkedList<wstring> result;

    vector<wchar_t *> container;
    CCSVParser::Parse(str, container, _T("|"));

    for (auto iter = container.begin(); iter != container.end(); ++iter) {
        result.push_back(*iter);
    }

    return result;
}

int CStuCSVParser::Parse(wchar_t *line, Student& inf)
{
    vector<wchar_t *> container;
    CCSVParser::Parse(line, container);

    inf.SetID(container.size() >= 1 ? container.at(0) : LIST_ERR_STR);
    inf.SetName(container.size() >= 2 ? container.at(1) : LIST_ERR_STR);
    inf.SetIsMale(container.size() >= 3 ? _wtoi(container.at(2)) : 0);
    inf.SetClass(container.size() >= 4 ? container.at(3) : LIST_ERR_STR);
    inf.SetPhoneNum(container.size() >= 5 ? container.at(4) : LIST_ERR_STR);
    if(container.size() >= 6 && wcscmp(container.at(5), _T("\n"))) {
        inf.SetChosenCourses(ParseChosenCourse(container.at(5)));
    }

    return container.size();
}

unsigned int CStuCSVParser::GetParsedLine()
{
    return parsedLine;
}

bool CStuCSVParser::HasExtraInf()
{
    return hasExtraInf;
}

bool CStuCSVParser::HasDataError()
{
    return hasDataError;
}