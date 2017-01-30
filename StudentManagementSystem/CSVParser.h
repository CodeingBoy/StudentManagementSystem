#pragma once

#include "LinkedListSel.h"

class CCSVParser
{
public:
    CCSVParser();
    ~CCSVParser();
    static LINKED_LIST<wchar_t*>& CCSVParser::Parse(wchar_t* line, LINKED_LIST<wchar_t*>& pvector);
    unsigned int GetParsedLine();
};

