#ifndef _jni_main_h_
#define _jni_main_h_
#include <jni.h>
#include "magic/magicmain.h"
#include "utils/fileutils.h"
#include <pthread.h>

void playSound(int soundId);
void playMusic(int musicId);

class AndroidFileUtils : public FileUtils{
public:
    virtual bool SaveImage( char* buffer, int w, int h, int format);
    virtual unsigned char* readRes( const char* resname, uint32_t& size );
};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj, jfloat delta);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setPreviewDataInfo(JNIEnv * env, jobject obj,  jint width, jint height, int format);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_uploadPreviewData(JNIEnv * env, jobject obj,  jbyteArray buffer);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setPreviewImage(JNIEnv * env, jobject obj,  jbyteArray path);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_rotate90Input(JNIEnv * env, jobject obj,  jboolean clockwise);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDown(JNIEnv * env, jobject obj, jfloat x, jfloat y);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDrag(JNIEnv * env, jobject obj, jfloat x, jfloat y);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchUp(JNIEnv * env, jobject obj, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setResPath(JNIEnv * env, jobject obj,  jbyteArray path);

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
static jmethodID getMethodID(const char *methodName, const char *paramCode);
void CheckException(const char* methond );

#ifdef __cplusplus
}
#endif

#endif // _jni_main_h_