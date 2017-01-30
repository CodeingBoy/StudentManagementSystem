#include "stdafx.h"
#include "StuCSVParser.h"

#define LIST_ERR -1

CStuCSVParser::CStuCSVParser(CFileHandler *pHandler)
{
    this->fileHandler = pHandler;
}

CStuCSVParser::~CStuCSVParser()
{
}

bool CStuCSVParser::Parse(bool haveHeader, std::vector<StudentInf> *plist)
{
    if (haveHeader)
        delete fileHandler->ReadLine(); // 忽略第一行

    StudentInf inf;
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

bool CStuCSVParser::Parse(bool haveHeader, CMyListCtrlExt *plist)
{
    if (haveHeader)
        delete fileHandler->ReadLine(); // 忽略第一行
    StudentInf inf;
    wchar_t line[1024];
    while (!(fileHandler->isEOF())) {
        memset(line, NULL, 1024); // 设置为空
        if (!fileHandler->ReadLine(line) || !wcscmp(line, _T("")))
            continue;
        int size = Parse(line, inf);
        plist->AddNewLine(inf);
        if (size < 6)
            hasDataError = true;
        else if(size > 6)
            hasExtraInf = true;
        parsedLine++;
    }

    return true;
}

int CStuCSVParser::Parse(wchar_t *line, StudentInf &inf)
{
    vector<wchar_t *> container;
    CCSVParser::Parse(line, container);

    inf.studentID = container.size() >= 1 ? _wtof(container.at(0)) : LIST_ERR;
    container.size() >= 2 ? wcscpy(inf.name, container.at(1)) : wcscpy(inf.name, _T("错误"));
    container.size() >= 3 ? wcscpy(inf.studentClass, container.at(2)) : wcscpy(inf.studentClass, _T("错误"));
    inf.mark_subject1 = container.size() >= 4 ? _wtof(container.at(3)) : LIST_ERR;
    inf.mark_subject2 = container.size() >= 5 ? _wtof(container.at(4)) : LIST_ERR;
    inf.mark_subject3 = container.size() >= 6 ? _wtof(container.at(5)) : LIST_ERR;
    inf.mark_total = container.size() >= 6 ? inf.mark_subject1 + inf.mark_subject2 + inf.mark_subject3 : LIST_ERR;

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