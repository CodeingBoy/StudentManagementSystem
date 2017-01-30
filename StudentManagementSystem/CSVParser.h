#pragma once

#include "StudentInf.h"
#include <vector>

using namespace std;

class CCSVParser
{
    public:
        CCSVParser();
        ~CCSVParser();
        static vector<wchar_t *> &CCSVParser::Parse(wchar_t *line, vector<wchar_t *> &pvector);
        unsigned int GetParsedLine();
};

