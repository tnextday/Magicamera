#pragma once

class IOCallBack{
public:
    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
    virtual unsigned char* readRes(const char* resname, uint32_t& size) = 0;
};
