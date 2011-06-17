#ifndef _jni_main_h_
#define _jni_main_h_
#include <jni.h>


void playSound(int soundId);
void playMusic(int musicId);

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

#ifdef __cplusplus
}
#endif

#endif // _jni_main_h_