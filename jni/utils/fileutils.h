#pragma once
#include <stdint.h>

#define  SafeDelete(p) {if(p) {delete p; p=NULL;}}            // for shorter code
#define  SafeDeleteArray(p) {if(p) {delete[] p; p=NULL;}}    // for shorter code

class FileUtils{
public:
    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
    //读取资源数据
    virtual unsigned char* readResFile(const char* resName, uint32_t& size) = 0;
    //读取普通数据
    virtual unsigned char* readNormalFile(const char* filePath, uint32_t& size);

    //************************************
    // Method:    readFile
    // Description: 文件读取函数，允许使用前缀{res://}
    // Parameter: [in] const char * filePath  文件路径，如果路径前缀为res:// 就从资源中读取数据，否则从文件中读取              
    // Parameter: [out] uint32_t & size 文件大小 
    // Returns:   unsigned char* 文件数据，如果文件不存在或者读取失败，返回null
    //************************************
    unsigned char* readFile(const char* filePath, uint32_t& size);  
};


unsigned char* EasyReadFile(const char* resname, uint32_t& size);
void setFileUtils(FileUtils *fu);
unsigned char* getFileDataFromZip( const char* pszZipFilePath, const char* pszFileName, unsigned int &size );

bool strStartWith(const char * str, const char * prefix);
