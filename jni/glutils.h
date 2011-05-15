#ifndef _glutils_h_
#define _glutils_h_

#include <android/log.h>
#include <GLES2/gl2.h>


#define  LOG_TAG    "libmagicjni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

static void printGLString(const char *name, GLenum s);
static void checkGlError(const char* op);
GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

#ifdef __cplusplus
}
#endif
#endif // _glutils_h_