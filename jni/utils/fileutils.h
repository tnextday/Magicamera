#pragma once
#include <stdint.h>

#define  SafeDelete(p) {if(p) {delete p; p=NULL;}}            // for shorter code
#define  SafeDeleteArray(p) {if(p) {delete[] p; p=NULL;}}    // for shorter code

class FileUtils{
public:
    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
    //��ȡ��Դ����
    virtual unsigned char* readResFile(const char* resName, uint32_t& size) = 0;
    //��ȡ��ͨ����
    virtual unsigned char* readNormalFile(const char* filePath, uint32_t& size);

    //************************************
    // Method:    readFile
    // Description: �ļ���ȡ����������ʹ��ǰ׺{res://}
    // Parameter: [in] const char * filePath  �ļ�·�������·��ǰ׺Ϊres:// �ʹ���Դ�ж�ȡ���ݣ�������ļ��ж�ȡ              
    // Parameter: [out] uint32_t & size �ļ���С 
    // Returns:   unsigned char* �ļ����ݣ�����ļ������ڻ��߶�ȡʧ�ܣ�����null
    //************************************
    unsigned char* readFile(const char* filePath, uint32_t& size);  
};


unsigned char* EasyReadFile(const char* resname, uint32_t& size);
void setFileUtils(FileUtils *fu);
unsigned char* getFileDataFromZip( const char* pszZipFilePath, const char* pszFileName, unsigned int &size );

bool strStartWith(const char * str, const char * prefix);
