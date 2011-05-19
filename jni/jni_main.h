#ifndef _jni_main_h_
#define _jni_main_h_
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj);


#ifdef __cplusplus
}
#endif

#endif // _jni_main_h_