#pragma once

#include <vector>
using namespace std;

class CCSVParser
{
public:
    CCSVParser();
    ~CCSVParser();
    static std::vector<wchar_t*>& CCSVParser::Parse(wchar_t* line, std::vector<wchar_t*>& pvector);
    unsigned int GetParsedLine();
};

