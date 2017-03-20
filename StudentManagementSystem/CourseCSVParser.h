#pragma once

#include "CSVParser.h"
#include "FileHandler.h"
#include "LinkedListSel.h"
#include "Course.h"

using namespace std;

class CCourseCSVParser :
    public CCSVParser
{
public:
    CCourseCSVParser(CFileHandler *pHandler);
    ~CCourseCSVParser();
    bool Parse(bool haveHeader, MyLinkedList<Course> *plist, const wchar_t *delimiter = NULL);
    static int Parse(wchar_t *line, Course& inf);
    unsigned int GetParsedLine();
    bool HasExtraInf();
    bool HasDataError();
private:
    bool hasLineError = false,
         hasDataError = false,
         hasExtraInf = false;
    CFileHandler *fileHandler;
    unsigned int parsedLine = 0;
};

