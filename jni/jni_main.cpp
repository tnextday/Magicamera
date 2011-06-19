#include <jni.h>
#include "jni_main.h"
#include "magicengine.h"

MagicEngine g_MagicEngine;

JNIEnv*     g_JniEnv = NULL;
jobject     g_JniObj;
jclass      g_JniClass;
jmethodID   g_JinMethod_playSound;
jmethodID   g_JinMethod_playMusic;

void initJavaCallBack(JNIEnv * env){
    g_JniEnv = env;
    g_JniClass = env->FindClass("com/funny/magicamera/MagicJNILib");
    jmethodID construction_id = env->GetMethodID(g_JniClass, "<init>", "()V"); 
    g_JniObj = env->NewObject(g_JniClass, construction_id); 

    g_JinMethod_playSound = env->GetMethodID(g_JniClass,"playSound","(I)V");
    g_JinMethod_playMusic = env->GetMethodID(g_JniClass,"playMusic","(I)V");
}

void playSound(int soundId){
    g_JniEnv->CallVoidMethod(g_JniObj, g_JinMethod_playSound, soundId); 
}

void playMusic(int musicId){
    g_JniEnv->CallVoidMethod(g_JniObj, g_JinMethod_playMusic, musicId); 
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    initJavaCallBack(env);
	g_MagicEngine.setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj, jfloat delta)
{
	g_MagicEngine.renderFrame(delta);
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



JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setSaveImagePath( JNIEnv * env, jobject obj, jstring path )
{
    char* strPath;
    strPath = (char*) env->GetStringChars(path, false);
    g_MagicEngine.setSaveImagePath(strPath);
    env->ReleaseStringChars(path, (jchar*)strPath);
}

