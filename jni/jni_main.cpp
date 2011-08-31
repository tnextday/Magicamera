#include "jni_main.h"
#include <string.h>

MagicMain g_MagicMain;
AndroidCallBack g_CallBack;

static JavaVM *g_JavaVM = 0;
static jclass classOfMagicJNILib = 0;
JNIEnv *env = 0;
/*
* Set some test stuff up.
*
* Returns the JNI version on success, -1 on failure.
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    g_JavaVM = vm;
    return JNI_VERSION_1_4;
}


JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    g_MagicMain.setupGraphics(width, height);
    g_MagicMain.setCallBack(&g_CallBack);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_step(JNIEnv * env, jobject obj, jfloat delta)
{
	g_MagicMain.renderFrame(delta);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setPreviewDataInfo(JNIEnv * env, jobject obj,  jint width, jint height, jint format){
	g_MagicMain.setPreviewDataInfo(width, height, format);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_uploadPreviewData(JNIEnv * env, jobject obj,  jbyteArray buffer, jlong len){
	char* p_buffer = (char*)env->GetPrimitiveArrayCritical(buffer, 0);
	g_MagicMain.updatePreviewData(p_buffer, len);
	env->ReleasePrimitiveArrayCritical(buffer, (char*)p_buffer, 0);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setPreviewImage( JNIEnv * env, jobject obj, jbyteArray path )
{
    char* strPath;
    strPath = (char*) env->GetPrimitiveArrayCritical(path, false);
    g_MagicMain.setPreviewImage(strPath);
    env->ReleasePrimitiveArrayCritical(path, strPath, false);
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDown( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
	return g_MagicMain.onTouchDown(x, y);
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchDrag( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
	return g_MagicMain.onTouchDrag(x, y);
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_MagicJNILib_onTouchUp( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
	return g_MagicMain.onTouchUp(x, y);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_MagicJNILib_setResPath( JNIEnv * env, jobject obj, jbyteArray path )
{
    char* strPath;
    strPath = (char*) env->GetPrimitiveArrayCritical(path, false);
    g_MagicMain.setResPath(strPath);
    g_MagicMain.loadRes();
    env->ReleasePrimitiveArrayCritical(path, strPath, false);
}


bool AndroidCallBack::SaveImage( char* buffer, int w, int h, int format )
{
    jmethodID jniMethod_saveImage = getMethodID("saveImage","([BIII)Z");
    if (!jniMethod_saveImage){
        return false;
    }
    int szBuffer = w*h*4;
    if (format == FORMAT_RGBA) {
        szBuffer = w*h*4;
    }
    jbyteArray jBuffer = env->NewByteArray(szBuffer);
    jbyte* p_buffer = (jbyte*)env->GetPrimitiveArrayCritical(jBuffer, 0);
    int szLine = w*4;
    for (int j = 0; j < h; j++){
        memcpy(p_buffer+szLine*j, buffer+szLine*(h-j-1), szLine);
    }
    env->ReleasePrimitiveArrayCritical(jBuffer, p_buffer, 0);
    bool result = env->CallStaticBooleanMethod(classOfMagicJNILib, jniMethod_saveImage, jBuffer, w, h, format);
    return result;
}

void CheckException(const char* methond )
{
    if (env->ExceptionOccurred()){
        LOGE("Jni Exception after %s\n", methond);
    }
}


static jmethodID getMethodID(const char *methodName, const char *paramCode)
{
    jmethodID ret = 0;
    // get jni environment and java class
    if (g_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){
        LOGE("Failed to get the environment using GetEnv()");
        return 0;
    }

    if (g_JavaVM->AttachCurrentThread(&env, 0) < 0){
        LOGE("Failed to get the environment using AttachCurrentThread()");
        return 0;
    }

    classOfMagicJNILib = env->FindClass("com/funny/magicamera/MagicJNILib");
    if (! classOfMagicJNILib){
        LOGE("Failed to find class of com/funny/magicamera/MagicJNILib");
        return 0;
    }

    if (env != 0 && classOfMagicJNILib != 0){
        ret = env->GetStaticMethodID(classOfMagicJNILib, methodName, paramCode);
    }

    if (! ret){
        LOGE("get method id of %s error", methodName);
    }

    return ret;
}



void playSound(int soundId){
    jmethodID jniMethod = getMethodID("playSound","(I)V");
    env->CallStaticVoidMethod(classOfMagicJNILib, jniMethod, soundId); 
}

void playMusic(int musicId){
    jmethodID jniMethod = getMethodID("playMusic","(I)V");
    env->CallStaticVoidMethod(classOfMagicJNILib, jniMethod, musicId); 
}



