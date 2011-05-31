#include <jni.h>
#include "jni_main.h"
#include "magicengine.h"

MagicEngine g_MagicEngine;


JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	g_MagicEngine.setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj)
{
	g_MagicEngine.renderFrame();
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setPreviewDataInfo(JNIEnv * env, jobject obj,  jint width, jint height, jint format){
	g_MagicEngine.setPreviewDataInfo(width, height, format);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_uploadPreviewData(JNIEnv * env, jobject obj,  jbyteArray buffer, jlong len){
	char* p_buffer = (char*)env->GetPrimitiveArrayCritical(buffer, 0);
	g_MagicEngine.updatePreviewTex(p_buffer, len);
	env->ReleasePrimitiveArrayCritical(buffer, (char*)p_buffer, 0);
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDown( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
	return g_MagicEngine.onTouchDown(x, y);
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDrag( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
	return g_MagicEngine.onTouchDrag(x, y);
}


JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchUp( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
	return g_MagicEngine.onTouchUp(x, y);
}

