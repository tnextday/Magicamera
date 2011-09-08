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

unsigned char* readRes( const char* resname, uint32_t& size )
{
    if (!g_FileUtils) return NULL;
    return g_FileUtils->readRes(resname, size);
}

void setFileUtils( FileUtils *fu )
{
    g_FileUtils = fu;
}