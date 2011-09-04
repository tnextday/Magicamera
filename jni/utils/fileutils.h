#pragma once


class FileUtils{ 
public:

    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
    virtual unsigned char* readRes(const char* resname, uint32_t& size) = 0;
};

unsigned char* getFileDataFromZip( const char* pszZipFilePath, const char* pszFileName, unsigned int &size );
