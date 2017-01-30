#pragma once

#include "CSVParser.h"
#include "FileHandler.h"
#include "Student.h"
#include "LinkedListSel.h"

using namespace std;

class CStuCSVParser :
    public CCSVParser
{
public:
    CStuCSVParser(CFileHandler* pHandler);
    ~CStuCSVParser();
    bool Parse(bool haveHeader, LINKED_LIST<Student>* plist);
    static int Parse(wchar_t* line, Student& inf);
    unsigned int GetParsedLine();
    bool HasExtraInf();
    bool HasDataError();
private:
    bool hasLineError = false,
         hasDataError = false,
         hasExtraInf = false;
    CFileHandler* fileHandler;
    unsigned int parsedLine = 0;
};

