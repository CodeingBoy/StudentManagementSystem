#include "CSVParser.h"
#include <TCHAR.h>
#include <vector>

using namespace std;

CCSVParser::CCSVParser()
{
}


CCSVParser::~CCSVParser()
{
}

vector<wchar_t *>& CCSVParser::Parse(wchar_t *line, vector<wchar_t *>& container, const wchar_t *delimiter)
{
    // �ָ��ַ���
    wchar_t *pStr, *pContext = nullptr;
    int times = 0;
    pStr = wcstok_s(line, delimiter, &pContext);

    bool hasLineError = false;
    while (pStr) {
        container.push_back(pStr);
        pStr = wcstok_s(pContext, delimiter, &pContext);
    }
    container.shrink_to_fit();

    return container;
}

//vector<wchar_t *>& CCSVParser::Parse(wchar_t *line, vector<wchar_t *>& container)
//{
//    // �ָ��ַ���
//    wchar_t *pStr, *pContext = nullptr;
//    int times = 0;
//    pStr = wcstok_s(line, _T(","), &pContext);
//
//    bool hasLineError = false;
//    while (pStr) {
//        container.push_back(pStr);
//        pStr = wcstok_s(pContext, _T(","), &pContext);
//    }
//    container.shrink_to_fit();
//
//    return container;
//}



