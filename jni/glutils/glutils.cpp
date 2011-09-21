
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glutils.h"

#ifdef _WIN32
void OutputDebugText(const char* sz, ...){
    char szData[512] = {0};
    va_list args;
    va_start(args, sz);
    _vsnprintf(szData, sizeof(szData)-1, sz, args);
    va_end(args);
    OutputDebugStringA(szData);
}
#endif

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}


void printGLColorSpaceInfo(){
    GLint param;
    glGetIntegerv(GL_RED_BITS, &param);//缓冲red位数
    LOGI("Red bits: %d\n", param);
    glGetIntegerv(GL_GREEN_BITS, &param);//缓冲green位数
    LOGI("Green bits: %d\n", param);
    glGetIntegerv(GL_BLUE_BITS, &param);
    LOGI("Blue bits: %d\n", param);
    glGetIntegerv(GL_ALPHA_BITS, &param);//缓冲Alpha位数
    LOGI("Alpha bits: %d\n", param);
}


const char* glErrorString(GLenum errcode)
{

    if (errcode == GL_NO_ERROR)
        return "GL_NO_ERROR";
    else if (errcode == GL_INVALID_ENUM)
        return"GL_INVALID_ENUM";
    else if (errcode == GL_INVALID_VALUE)
        return "GL_INVALID_VALUE";
    else if (errcode == GL_INVALID_OPERATION)
        return "GL_INVALID_OPERATION";
    else if (errcode == GL_OUT_OF_MEMORY)
        return "GL_OUT_OF_MEMORY";
    else if (errcode == GL_INVALID_FRAMEBUFFER_OPERATION)
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    else
        return "Unknow Error Code";
}



void checkGlError(const char* op) {
    int i = 0;
    for (GLint error = glGetError(); error; error
        = glGetError()) {
            LOGI("after %s glError (0x%x[%s]) No. %d\n", op, error, glErrorString(error), i);
            i++;
    }
}

bool checkIfSupportsExtension(const char *extension){
    const char *extensions = (const char *) glGetString(GL_EXTENSIONS);
    return strstr(extensions, extension) != NULL;
}
