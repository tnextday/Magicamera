#ifndef _glutils_h_
#define _glutils_h_

#include <GLES2/gl2.h>

#ifdef _WIN32
#include <stdio.h>
#	define  LOGI(...)  printf(__VA_ARGS__)
#	define  LOGE(...)  printf(__VA_ARGS__)
#else
#include <android/log.h>
#	define  LOG_TAG    "libmagicjni"
#	define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#	define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif // _WIN32


#ifdef __cplusplus
extern "C" {
#endif

GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);
bool checkIfSupportsExtension(const char *extension);

#ifdef __cplusplus
}
#endif
#endif // _glutils_h_