#include "fileutils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zip/unzip.h"

FileUtils *g_FileUtils = NULL;

unsigned char* getFileDataFromZip( const char* pszZipFilePath, const char* pszFileName, unsigned int &size )
{
    unsigned char * pBuffer = NULL;
    unzFile pFile = NULL;
    size = 0;

    do 
    {
        if(!pszZipFilePath || !pszFileName) break;
        if(strlen(pszZipFilePath) == 0) break;

        pFile = unzOpen(pszZipFilePath);
        if (!pFile) break;

        int nRet = unzLocateFile(pFile, pszFileName, 1);
        if (UNZ_OK != nRet) break;

        char szFilePathA[260];
        unz_file_info FileInfo;
        nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
        if (UNZ_OK != nRet) break;

        nRet = unzOpenCurrentFile(pFile);
        if (UNZ_OK != nRet) break;

        pBuffer = new unsigned char[FileInfo.uncompressed_size];
        int nSize = 0;
        nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
        assert(nSize == 0 || nSize == FileInfo.uncompressed_size);

        size = FileInfo.uncompressed_size;
        unzCloseCurrentFile(pFile);
    } while (0);

    if (pFile)
    {
        unzClose(pFile);
    }

    return pBuffer;
}

unsigned char* EasyReadFile( const char* resName, uint32_t& size )
{
    if (!g_FileUtils) return NULL;
    return g_FileUtils->readFile(resName, size);
}

void setFileUtils( FileUtils *fu )
{
    g_FileUtils = fu;
}

bool strStartWith( const char * str, const char * prefix )
{
    if (!str || !prefix)
        return false;
    while(*str){
        if (*str != *prefix)
            break;
        str++; 
        prefix++;
        if (*prefix == 0)
            return true;
    }
    return false;
}

unsigned char* FileUtils::readFile( const char* filePath, uint32_t& size )
{
    if (strStartWith(filePath, "res://"))
        return readResFile(filePath+6, size);
    else
        return readNormalFile(filePath, size);
}

unsigned char* FileUtils::readNormalFile( const char* filePath, uint32_t& size )
{
    FILE *fp = fopen(filePath, "rb");
    if (!fp) return NULL;
    unsigned char* buffer = NULL;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);            // determine file size so we can fill it in later if FileSize == 0
    if (size <= 0) goto __end ;
    fseek(fp, 0, SEEK_SET);
    buffer = new unsigned char[size];
    if (fread(buffer, sizeof(char), size, fp) <= 0){
        SafeDeleteArray(buffer);
    }

__end:
    fclose(fp);
    return buffer;
}