#include "StuCSVParser.h"
#include <TCHAR.h>
#include <vector>

#define LIST_ERR -1
#define LIST_ERR_STR (_T("Error"))

CStuCSVParser::CStuCSVParser(CFileHandler* pHandler)
{
    this->fileHandler = pHandler;
}

CStuCSVParser::~CStuCSVParser()
{
}

bool CStuCSVParser::Parse(bool haveHeader, LINKED_LIST<Student>* plist)
{
    if (haveHeader)
        delete fileHandler->ReadLine(); // 忽略第一行

    Student inf;
    wchar_t line[1024];
    while (!fileHandler->isEOF()) {
        memset(line, NULL, 1024); // 设置为空
        if (!fileHandler->ReadLine(line) || !wcscmp(line, _T("")))
            continue;
        int size = Parse(line, inf);
        plist->push_back(inf);
        if (size < 6)
            hasDataError = true;
        else if (size > 6)
            hasExtraInf = true;
        parsedLine++;
    }

    return true;
}

int CStuCSVParser::Parse(wchar_t* line, Student& inf)
{
    vector<wchar_t*> container;
    CCSVParser::Parse(line, container);

    inf.SetID(container.size() >= 1 ? container.at(0) : LIST_ERR_STR);
    inf.SetName(container.size() >= 2 ? container.at(1) : LIST_ERR_STR);
    inf.SetIsMale(container.size() >= 4 ? _wtoi(container.at(2)) : 0);
    inf.SetClass(container.size() >= 3 ? container.at(3) : LIST_ERR_STR);
    inf.SetPhoneNum(container.size() >= 5 ? container.at(4) : LIST_ERR_STR);

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