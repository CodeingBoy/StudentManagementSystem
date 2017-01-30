#include "CSVParser.h"
#include <TCHAR.h>


CCSVParser::CCSVParser()
{
}


CCSVParser::~CCSVParser()
{
}

LINKED_LIST<wchar_t*>& CCSVParser::Parse(wchar_t* line, LINKED_LIST<wchar_t*>& container)
{
    // ·Ö¸î×Ö·û´®
    wchar_t* pStr, *pContext = nullptr;
    int times = 0;
    pStr = wcstok_s(line, _T(","), &pContext);

    bool hasLineError = false;
    while (pStr) {
        container.push_back(pStr);
        pStr = wcstok_s(pContext, _T(","), &pContext);
    }
    container.shrink_to_fit();

    return container;
}



