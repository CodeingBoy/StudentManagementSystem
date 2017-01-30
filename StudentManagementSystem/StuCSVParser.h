#pragma once

#include "CSVParser.h"
#include "FileHandler.h"
#include "MyListCtrlExt.h"

#include <vector>

using namespace std;

class CStuCSVParser :
    public CCSVParser
{
    public:
        CStuCSVParser(CFileHandler *pHandler);
        ~CStuCSVParser();
        bool Parse(bool haveHeader, std::vector<StudentInf> *plist);
        bool Parse(bool haveHeader, CMyListCtrlExt *plist);
        static int Parse(wchar_t *line, StudentInf &inf);
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

