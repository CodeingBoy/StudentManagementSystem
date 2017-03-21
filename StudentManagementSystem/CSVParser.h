#pragma once

#include <vector>
#include <TCHAR.h>
using namespace std;

class CCSVParser
{
public:
    CCSVParser();
    ~CCSVParser();
    static vector<wchar_t *>& Parse(wchar_t *line, vector<wchar_t *>& container, const wchar_t *delimiter = _T(","));
    //static vector<wchar_t *>& CCSVParser::Parse(wchar_t *line, vector<wchar_t *>& container);
    unsigned int GetParsedLine();
};

