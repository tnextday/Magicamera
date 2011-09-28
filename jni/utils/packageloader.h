#pragma once
#include <stdint.h>

struct BlendTexFileHeader{
    char    sgin[4];
    uint32_t  sfactor;
    uint32_t  dfactor;
    uint32_t  size;
};

class BlendTexLoader{
    BlendTexFileHeader  *m_fheader;
    char*               m_fileBuffer;
    char*               m_data;
public:
    BlendTexLoader();
    ~BlendTexLoader();

    bool loadFromFile(const char* path);

    uint32_t getSrcFactor();
    uint32_t getDstFactor();
    unsigned char*  getData();
    uint32_t  getDataSize();
protected:
    bool loadFormBuffer(const char* buffer, const uint32_t size);
};