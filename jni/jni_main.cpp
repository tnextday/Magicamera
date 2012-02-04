#include "jni_main.h"
#include <string.h>

MagicMain *g_MagicMain = NULL;
AndroidFileUtils g_androidFileUtils;

static JavaVM *g_JavaVM = 0;
static jclass classOfCoreJNILib = 0;
JNIEnv *env = 0;



bool AndroidFileUtils::SaveImage( char* buffer, int w, int h, int format )
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
    bool result = env->CallStaticBooleanMethod(classOfCoreJNILib, jniMethod_saveImage, jBuffer, w, h, format);
    env->ReleaseByteArrayElements(jBuffer, p_buffer, szBuffer);
    return result;
}


void AndroidFileUtils::setApkPath( const char * path )
{
    ApkPath[MAX_PATH-1] = '\0';
    strncpy(ApkPath, path, MAX_PATH - 1);
}

unsigned char* AndroidFileUtils::readResFile( const char* resName, uint32_t& size )
{
    char respath[MAX_PATH] = {0};
    strcpy(respath, "assets/");
    strncat(respath, resName, MAX_PATH - 8 );
//     LOGI("LoadRes:[%s][%s]", ApkPath, respath);
    return getFileDataFromZip(ApkPath, respath, size);
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

    classOfCoreJNILib = env->FindClass("com/funny/magicamera/CoreJNILib");
    if (! classOfCoreJNILib){
        LOGE("Failed to find class of com/funny/magicamera/CoreJNILib");
        return 0;
    }

    if (env != 0 && classOfCoreJNILib != 0){
        ret = env->GetStaticMethodID(classOfCoreJNILib, methodName, paramCode);
    }

    if (! ret){
        LOGE("get method id of %s error", methodName);
    }

    return ret;
}



void playSound(int soundId){
    jmethodID jniMethod = getMethodID("playSound","(I)V");
    env->CallStaticVoidMethod(classOfCoreJNILib, jniMethod, soundId); 
}

void playMusic(int musicId){
    jmethodID jniMethod = getMethodID("playMusic","(I)V");
    env->CallStaticVoidMethod(classOfCoreJNILib, jniMethod, musicId); 
}

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


JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_create(JNIEnv * env, jobject obj)
{
    g_MagicMain = new MagicMain();
    g_MagicMain->setupGraphics();
    g_MagicMain->setIOCallBack(&g_androidFileUtils);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_destory(JNIEnv * env, jobject obj){
    SafeDelete(g_MagicMain);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_resize( JNIEnv * env, jobject obj, jint width, jint height )
{
    if (g_MagicMain)
        g_MagicMain->resize(width, height);
}


JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_step(JNIEnv * env, jobject obj, jfloat delta)
{
    if (g_MagicMain)
	    g_MagicMain->renderFrame(delta);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setInputDataInfo(JNIEnv * env, jobject obj,  jint width, jint height, jint format)
{
    if (g_MagicMain)
	    g_MagicMain->setInputDataInfo(width, height, format);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_updateInputData(JNIEnv * env, jobject obj,  jbyteArray buffer)
{
	char* p_buffer = (char*)env->GetPrimitiveArrayCritical(buffer, 0);
    long len = env->GetArrayLength(buffer);
    if (g_MagicMain)
	    g_MagicMain->updateInputData(p_buffer, len);
	env->ReleasePrimitiveArrayCritical(buffer, (char*)p_buffer, 0);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setInputImage( JNIEnv * env, jobject obj, jstring path )
{
    const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(path, &isCopy);
    if (isCopy ) {
        //pthread_mutex_lock(&g_lock);
        if (g_MagicMain)
            g_MagicMain->setInputImage(str);
        //pthread_mutex_unlock(&g_lock);
        env->ReleaseStringUTFChars(path, str);
    }
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_CoreJNILib_onTouchDown( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
    if (g_MagicMain)
	    return g_MagicMain->onTouchDown(x, y);
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_CoreJNILib_onTouchDrag( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
    if (g_MagicMain)
	    return g_MagicMain->onTouchDrag(x, y);
}

JNIEXPORT jboolean JNICALL Java_com_funny_magicamera_CoreJNILib_onTouchUp( JNIEnv * env, jobject obj, jfloat x, jfloat y )
{
    if (g_MagicMain)
	    return g_MagicMain->onTouchUp(x, y);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_rotate90( JNIEnv * env, jobject obj, jboolean clockwise )
{
    if (g_MagicMain)
        g_MagicMain->rotate90Input(clockwise);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_flip(JNIEnv * env, jobject obj,  jboolean x, jboolean y){
    if (g_MagicMain)
        g_MagicMain->flip(x, y);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_resetRotation( JNIEnv * env, jobject obj)
{
    if (g_MagicMain)
        g_MagicMain->resetRotation();
}


JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setApkPath( JNIEnv * env, jobject obj, jstring apkPath )
{
    const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(apkPath, &isCopy);
    if (isCopy) {
        g_androidFileUtils.setApkPath(str);
        env->ReleaseStringUTFChars(apkPath, str);
    }
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_takePicture( JNIEnv * env, jobject obj )
{
    if (g_MagicMain)
        g_MagicMain->takePicture();
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_takePictureWithFile( JNIEnv * env, jobject obj, jstring path )
{
    const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(path, &isCopy);
    if (isCopy) {
        if (g_MagicMain)
            g_MagicMain->takePicture(str);
        env->ReleaseStringUTFChars(path, str);
    }
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_takePictureWithBuffer( JNIEnv * env, jobject obj, jbyteArray buffer )
{
    char* p_buffer = (char*)env->GetPrimitiveArrayCritical(buffer, 0);
    long len = env->GetArrayLength(buffer);
    if (g_MagicMain)
        g_MagicMain->takePicture(p_buffer, len);
    env->ReleasePrimitiveArrayCritical(buffer, (char*)p_buffer, 0);
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setCover( JNIEnv * env, jobject obj, jstring path )
{
    const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(path, &isCopy);
    if (isCopy) {
        if (g_MagicMain)
            g_MagicMain->setCover(str);
        env->ReleaseStringUTFChars(path, str);
    }
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_switchEngine( JNIEnv * env, jobject obj, jint type )
{
    if (g_MagicMain)
        g_MagicMain->switchEngine((EngineType)type);
}

JNIEXPORT jint JNICALL Java_com_funny_magicamera_CoreJNILib_getEngineType( JNIEnv * env, jobject obj )
{
    int type = 0;
    if (g_MagicMain)
        type = (int)g_MagicMain->getEngineType();
    return type;
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_restoreMesh( JNIEnv * env, jobject obj )
{
    if (g_MagicMain)
        g_MagicMain->restoreMesh();
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setFrame( JNIEnv * env, jobject obj, jstring path )
{
    const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(path, &isCopy);
    if (isCopy) {
        if (g_MagicMain)
            g_MagicMain->setFrame(str);
        env->ReleaseStringUTFChars(path, str);
    }
}

JNIEXPORT void JNICALL Java_com_funny_magicamera_CoreJNILib_setEffect( JNIEnv * env, jobject obj, jstring name )
{
    const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(name, &isCopy);
    if (isCopy) {
        if (g_MagicMain)
            g_MagicMain->setEffect(str);
        env->ReleaseStringUTFChars(name, str);
    }
}

JNIEXPORT jstring JNICALL Java_com_funny_magicamera_CoreJNILib_getEffectList(JNIEnv * env, jobject obj)
{
    return env->NewStringUTF(g_MagicMain->getEffectList());
}