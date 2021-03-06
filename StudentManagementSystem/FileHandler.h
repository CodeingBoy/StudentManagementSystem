#pragma once
#include <cstdio>

class CFileHandler
{
public:
    CFileHandler(const wchar_t *filename, bool bRead);
    ~CFileHandler();
    bool ReadLine(wchar_t *output);
    wchar_t *ReadLine();
    bool Write(const wchar_t *content);
    bool WriteLine(const wchar_t *content);
    int GetError();
    bool isEOF();
    bool IsVaild();
private:
    bool bRead = true; // ��ȡ���Ǳ���
    int err;
    FILE *fp;
};

