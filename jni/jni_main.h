#ifndef _jni_main_h_
#define _jni_main_h_
#include <jni.h>
#include "magicengine.h"

void playSound(int soundId);
void playMusic(int musicId);

class AndroidCallBack : public SaveImageCallBack{
public:
    virtual bool SaveImage( char* buffer, int w, int h, int format);
};

class AndroidMethod{
    JNIEnv*     m_JniEnv;
    jobject     m_JniObj;
    jclass      m_JniClass;
    jmethodID   m_JniMethod_playSound;
    jmethodID   m_JniMethod_playMusic;
    jmethodID   m_JniMethod_saveImage;
public:
    AndroidMethod();
    void initJavaCallBack(JNIEnv * env);
    void playSound(int soundId);
    void playMusic(int musicId);
    bool saveImage(char* buffer, int w, int h, int format);

};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj, jfloat delta);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setPreviewDataInfo(JNIEnv * env, jobject obj,  jint width, jint height, int format);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_uploadPreviewData(JNIEnv * env, jobject obj,  jbyteArray buffer, jlong len);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDown(JNIEnv * env, jobject obj, jfloat x, jfloat y);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDrag(JNIEnv * env, jobject obj, jfloat x, jfloat y);
JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchUp(JNIEnv * env, jobject obj, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setSaveImagePath(JNIEnv * env, jobject obj,  jbyteArray path);

#ifdef __cplusplus
}
#endif

#endif // _jni_main_h_