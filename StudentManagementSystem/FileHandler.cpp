#include "stdafx.h"
#include "FileHandler.h"


CFileHandler::CFileHandler(const wchar_t *filename, bool bRead)
{
    if (bRead) {
        if (err = _wfopen_s(&fp, filename, _T("r,ccs=UTF-8")))   // err
            return;
    } else {
        if (err = _wfopen_s(&fp, filename, _T("w,ccs=UTF-8")))  // err
            return;
    }
}


CFileHandler::~CFileHandler()
{
    if (fp)
        fclose(fp);
}

bool CFileHandler::ReadLine(wchar_t *output)
{
    if (!fp || feof(fp))return false;
    fgetws(output, 1024, fp);  //∂¡»°“ª––
    return true;
}

wchar_t *CFileHandler::ReadLine()
{
    if (!fp || feof(fp))return false;
    wchar_t *output = new wchar_t[1024];
    memset(output, 0, 1024);
    ReadLine(output);
    return output;
}

bool CFileHandler::Write(wchar_t *content)
{
    if (!fp || feof(fp))return false;
    fputws(content, fp);
    return true;
}

bool CFileHandler::WriteLine(wchar_t *content)
{
    if (!Write(content))
        return false;
    Write(_T("\n"));
    return true;
}

int CFileHandler::GetError()
{
    return err;
}

bool CFileHandler::isEOF()
{
    return feof(fp);
}