#pragma once
#include <stdio.h>

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

#ifdef _WIN32
#define snprintf _snprintf
#endif

void setResPath(const char* path);
char* makeResPath(char* path, const char* targetFile, int szBuffer = _MAX_PATH);

char* readRes(const char* resname, long* size);

