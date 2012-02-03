#ifndef _jni_main_h_
#define _jni_main_h_
#include <jni.h>
#include "magic/magicmain.h"
#include "utils/fileutils.h"
#include <pthread.h>

void playSound(int soundId);
void playMusic(int musicId);

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

class AndroidFileUtils : public FileUtils{
    char    ApkPath[MAX_PATH];
public:
    AndroidFileUtils(){ApkPath[0] = '\0';};
    virtual bool SaveImage( char* buffer, int w, int h, int format);
    virtual unsigned char* readResFile( const char* resName, uint32_t& size );

    void setApkPath(const char * path);
};

#define ENGINE_TYPE_NONE 0
#define ENGINE_TYPE_MESH 1
#define ENGINE_TYPE_COVER 2
#define ENGINE_TYPE_Kaleidoscope 3

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_create(JNIEnv * env, jobject obj);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_destory(JNIEnv * env, jobject obj);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_resize(JNIEnv * env, jobject obj,  jint width, jint height);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_step(JNIEnv * env, jobject obj, jfloat delta);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_switchEngine(JNIEnv * env, jobject obj, jint type);
JNIEXPORT jint JNICALL Java_com_funny_magicamera_CoreJNILib_getEngineType(JNIEnv * env, jobject obj);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_takePicture(JNIEnv * env, jobject obj);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_takePictureWithFile(JNIEnv * env, jobject obj, jstring path);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_takePictureWithBuffer(JNIEnv * env, jobject obj, jbyteArray buffer);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setCover(JNIEnv * env, jobject obj, jstring path);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setFrame(JNIEnv * env, jobject obj, jstring path);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setEffect(JNIEnv * env, jobject obj, jstring name);
JNIEXPORT jstring JNICALL Java_com_funny_magicamera_CoreJNILib_getEffectList(JNIEnv * env, jobject obj);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_restoreMesh(JNIEnv * env, jobject obj);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setInputDataInfo(JNIEnv * env, jobject obj,  jint width, jint height, int format);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_updateInputData(JNIEnv * env, jobject obj,  jbyteArray buffer);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setInputImage(JNIEnv * env, jobject obj,  jstring path);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_rotate90(JNIEnv * env, jobject obj,  jboolean clockwise);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_flip(JNIEnv * env, jobject obj,  jboolean x, jboolean y);
JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_resetRotation(JNIEnv * env, jobject obj);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_CoreJNILib_onTouchDown(JNIEnv * env, jobject obj, jfloat x, jfloat y);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_CoreJNILib_onTouchDrag(JNIEnv * env, jobject obj, jfloat x, jfloat y);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_CoreJNILib_onTouchUp(JNIEnv * env, jobject obj, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setApkPath(JNIEnv * env, jobject obj, jstring apkPath );

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
static jmethodID getMethodID(const char *methodName, const char *paramCode);
void CheckException(const char* methond );

#ifdef __cplusplus
}
#endif

#endif // _jni_main_h_