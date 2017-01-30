#pragma once
#include <cstdio>

class CFileHandler
{
public:
    CFileHandler(const wchar_t* filename, bool bRead);
    ~CFileHandler();
    bool ReadLine(wchar_t* output);
    wchar_t* ReadLine();
    bool Write(wchar_t* content);
    bool WriteLine(wchar_t* content);
    int GetError();
    bool isEOF();
private:
    bool bRead = true; // 读取还是保存
    int err;
    FILE* fp;
};

