#include <TCHAR.h>
#include <vector>
#include "CourseCSVParser.h"

#define LIST_ERR -1
#define LIST_ERR_STR (_T("Error"))

CCourseCSVParser::CCourseCSVParser(CFileHandler *pHandler)
{
    this->fileHandler = pHandler;
}

CCourseCSVParser::~CCourseCSVParser()
{
}

bool CCourseCSVParser::Parse(bool haveHeader, MyLinkedList<Course> *plist, const wchar_t *delimiter)
{
    if (haveHeader)
        delete fileHandler->ReadLine(); // 忽略第一行

    Course inf;
    wchar_t line[1024];
    while (!fileHandler->isEOF()) {
        memset(line, NULL, 1024); // 设置为空
        if (!fileHandler->ReadLine(line) || !wcscmp(line, _T("")))
            continue;
        if (delimiter != NULL && !wcscmp(line, delimiter))
            return true;
        int size = Parse(line, inf);
        plist->push_back(inf);
        if (size < 5)
            hasDataError = true;
        else if (size > 5)
            hasExtraInf = true;
        parsedLine++;
    }

    return true;
}

int CCourseCSVParser::Parse(wchar_t *line, Course& inf)
{
    vector<wchar_t *> container;
    CCSVParser::Parse(line, container);

    inf.SetID(container.size() >= 1 ? container.at(0) : LIST_ERR_STR);
    inf.SetName(container.size() >= 2 ? container.at(1) : LIST_ERR_STR);
    inf.SetPeriod(container.size() >= 3 ? container.at(2) : LIST_ERR_STR);
    inf.SetTeacherName(container.size() >= 4 ? container.at(3) : LIST_ERR_STR);

    return container.size();
}

unsigned int CCourseCSVParser::GetParsedLine()
{
    return parsedLine;
}

bool CCourseCSVParser::HasExtraInf()
{
    return hasExtraInf;
}

bool CCourseCSVParser::HasDataError()
{
    return hasDataError;
}