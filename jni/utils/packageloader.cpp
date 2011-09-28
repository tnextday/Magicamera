#include "PackageLoader.h"
#include "utils/fileutils.h"
#include <string.h>

BlendTexLoader::BlendTexLoader()
{
    m_fheader = NULL;
    m_data = NULL;
    m_fileBuffer = NULL;
}

BlendTexLoader::~BlendTexLoader()
{
    SafeDeleteArray(m_fileBuffer);
}

bool BlendTexLoader::loadFromFile( const char* path )
{
    uint32_t filesize;
    m_fileBuffer = (char*)EasyReadFile(path, filesize);
    bool ret = loadFormBuffer(m_fileBuffer, filesize);
    if (!ret){
        SafeDeleteArray(m_fileBuffer);
    }
    return ret;
}

bool BlendTexLoader::loadFormBuffer( const char* buffer, const uint32_t size )
{
    if (!buffer || !size)
        return false;
    if (size <= sizeof(BlendTexFileHeader))
        return false;
    m_fheader = (BlendTexFileHeader*)buffer;

    if (m_fheader->sgin[0] == 'b' && m_fheader->sgin[1] == 'f' && m_fheader->sgin[2] == 't' ){
        if (m_fheader->size && m_fheader->size + sizeof(BlendTexFileHeader) <= size){
            m_data = (char*)buffer+sizeof(BlendTexFileHeader);
            return true;
        }
    }
    return false;
}

uint32_t BlendTexLoader::getSrcFactor()
{
    if (m_fheader)
        return m_fheader->sfactor;
    else
        return 0;
}

uint32_t BlendTexLoader::getDstFactor()
{   
    if (m_fheader)
        return m_fheader->dfactor;
    else
        return 0;
}

unsigned char* BlendTexLoader::getData()
{
    return (unsigned char*)m_data;
}

uint32_t BlendTexLoader::getDataSize()
{
    if (m_fheader)
        return m_fheader->size;
    else
        return 0;
}