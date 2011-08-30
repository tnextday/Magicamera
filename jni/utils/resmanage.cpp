#include "resmanage.h"

char g_ResPath[_MAX_PATH];

void setResPath(const char* path )
{
    snprintf(g_ResPath, _MAX_PATH-1, "%s", path);
}

char* makeResPath( char* path, const char* targetFile, int szBuffer/* = _MAX_PATH*/)
{
    snprintf(path, szBuffer, "%s/%s", g_ResPath, targetFile);
    return path;
}

char* readRes( const char* resname, long* size)
{
    char path[_MAX_PATH];
    *size = 0;
    FILE *fp = fopen(makeResPath(path, resname), "rb");
    if (!fp) {
        return NULL;
    }
    char* buffer = NULL;
    fseek(fp, 0, SEEK_END);
    long buffersize = ftell(fp);            // determine file size so we can fill it in later if FileSize == 0
    if (buffersize <= 0) {
        fclose(fp);
        return NULL;
    }
    fseek(fp, 0, SEEK_SET);
    buffer = new char[buffersize];

    if (fread(buffer, sizeof(char), buffersize, fp) <= 0){
         delete [] buffer; 
         buffer = NULL;
    }
    fclose(fp);
    *size = buffersize;
    return buffer;
}