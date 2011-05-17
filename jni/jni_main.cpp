#include <jni.h>

#include "magicengine.h"

MagicEngine g_MagicEngine;

extern "C" {
	JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
	JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	g_MagicEngine.setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj)
{
	g_MagicEngine.renderFrame();
}
