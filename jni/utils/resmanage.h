#pragma once
#include <stdio.h>

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

#ifdef _WIN32
#define snprintf _snprintf
#endif

class IOCallBack{
public:
    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
    virtual char* readRes(const char* resname, long* size) = 0;
};

void setResPath(const char* path);
char* makeResPath(char* path, const char* targetFile, int szBuffer = _MAX_PATH);

char* readRes(const char* resname, long* size);

