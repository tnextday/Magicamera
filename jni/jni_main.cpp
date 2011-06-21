#include "jni_main.h"
#include <string.h>

MagicEngine g_MagicEngine;
AndroidMethod g_AndroidMethod;
AndroidCallBack g_CallBack;


JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    g_MagicEngine.setupGraphics(width, height);
    g_AndroidMethod.initJavaCallBack(env);
    g_MagicEngine.setCallBack(&g_CallBack);
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

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setSaveImagePath( JNIEnv * env, jobject obj, jbyteArray path )
{
    char* strPath;
    strPath = (char*) env->GetPrimitiveArrayCritical(path, false);
    g_MagicEngine.setSaveImagePath(strPath);
    env->ReleasePrimitiveArrayCritical(path, strPath, false);
}


bool AndroidCallBack::SaveImage( char* buffer, int w, int h, int format )
{
    return g_AndroidMethod.saveImage(buffer, w, h, format);
}


AndroidMethod::AndroidMethod()
{
    m_JniEnv = NULL;
    m_JavaVM = NULL;
}


void AndroidMethod::initJavaCallBack(JNIEnv * env){
    m_JniEnv = env;
    m_JniEnv->GetJavaVM(&m_JavaVM);
    m_JvmThread = pthread_self(); //记住当前JNI环境的线程
    m_JniClass = env->FindClass("com/funny/magicamera/MagicJNILib");
    jmethodID construction_id = env->GetMethodID(m_JniClass, "<init>", "()V"); 
    m_JniObj = env->NewObject(m_JniClass, construction_id); 
    m_JniMethod_playSound = env->GetMethodID(m_JniClass,"playSound","(I)V");
    m_JniMethod_playMusic = env->GetMethodID(m_JniClass,"playMusic","(I)V");
    m_JniMethod_saveImage = env->GetMethodID(m_JniClass,"saveImage","([BIII)Z");
}

void AndroidMethod::playSound(int soundId){
    m_JniEnv->CallVoidMethod(m_JniObj, m_JniMethod_playSound, soundId); 
}

void AndroidMethod::playMusic(int musicId){
    m_JniEnv->CallVoidMethod(m_JniObj, m_JniMethod_playMusic, musicId); 
}

bool AndroidMethod::saveImage( char* buffer, int w, int h, int format )
{
    bool bAttachThread = false;
    if(m_JvmThread != pthread_self()){
        m_JavaVM->AttachCurrentThread(&m_JniEnv, NULL);
        bAttachThread = true;
    }
    int szBuffer = w*h*4;
    if (format == FORMAT_RGBA) {
        szBuffer = w*h*4;
    }
    jbyteArray jBuffer = m_JniEnv->NewByteArray(szBuffer);
    jbyte* p_buffer = (jbyte*)m_JniEnv->GetPrimitiveArrayCritical(jBuffer, 0);
    int szLine = w*4;
    for (int j = 0; j < h; j++){
        memcpy(p_buffer+szLine*j, buffer+szLine*(h-j-1), szLine);
    }
    m_JniEnv->ReleasePrimitiveArrayCritical(jBuffer, (void*)p_buffer, 0);
    bool result = m_JniEnv->CallBooleanMethod(m_JniObj, m_JniMethod_saveImage, jBuffer, w, h, format);
    if (bAttachThread)
        m_JavaVM->DetachCurrentThread();
    return result;
}
