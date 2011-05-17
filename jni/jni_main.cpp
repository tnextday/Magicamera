#include <jni.h>

#include "glengine.h"

GLEngine glEngine;

extern "C" {
	JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
	JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	glEngine.setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj)
{
	glEngine.renderFrame();
}
