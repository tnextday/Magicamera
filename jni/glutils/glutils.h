#ifndef _glutils_h_
#define _glutils_h_

#ifdef __MAC_NA
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#endif

#ifdef _WIN32
#include <stdio.h>
#include <Windows.h>
void OutputDebugText(const char* sz, ...);
#    define  LOGI(...)  OutputDebugText(__VA_ARGS__)
#    define  LOGE(...)  OutputDebugText(__VA_ARGS__)
#eles if def _ANDROID
#include <android/log.h>
#    define  LOG_TAG    "libmagic"
#    define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#    define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#include <stdio.h>
#    define  LOGI(...)  printf(__VA_ARGS__)
#    define  LOGE(...)  printf(__VA_ARGS__)
#endif // _WIN32

#define  SafeDelete(p) {if(p) {delete p; p=NULL;}}            // for shorter code
#define  SafeDeleteArray(p) {if(p) {delete[] p; p=NULL;}}    // for shorter code

#ifdef __cplusplus
extern "C" {
#endif

void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);
bool checkIfSupportsExtension(const char *extension);
void printGLColorSpaceInfo();
#ifdef __cplusplus
}
#endif
#endif // _glutils_h_