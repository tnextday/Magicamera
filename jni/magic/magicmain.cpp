#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "magicmain.h"
#include "glutils/glutils.h"
#include "utils/mathelpers.h"
#include "effectengine.h"
#include "effect/effectfactory.h"

const float MaxDeltaTime = 1.0/20.0;

static const char gVertexShader[] = 
        "uniform mat4 uMVPMatrix;\n"
        "attribute vec4 aPosition;\n"
        "attribute vec2 aTexCoord;\n"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "  gl_Position = uMVPMatrix * aPosition;\n" 
        "  vTexCoord = aTexCoord;\n"
        "}\n";

static const char gFragmentShader[] = 
        "precision mediump float;\n"
        "varying vec2 vTexCoord;\n"
        "uniform sampler2D sTexture;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(sTexture, vTexCoord);\n"
        "}\n";

MagicMain::MagicMain()
{
    m_glYUVTex = NULL;
    m_ioCallBack = NULL;
    m_Engine = NULL;
    m_nextEngine = EngineType_None;
    m_aspectRatio = 1;
    m_SrcTex = NULL;
}

MagicMain::~MagicMain()
{
    SafeDelete(m_glYUVTex);
    SafeDelete(m_Engine);
    SafeDelete(m_SrcTex);
}


bool MagicMain::setupGraphics() {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
    GLuint dims[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, (GLint*)dims);
    LOGI("MAX_VIEWPORT: %d, %d\n", dims[0], dims[1]);
    setMaxOutputSize(dims[0], dims[1]);

    m_shader.makeProgram(gVertexShader, gFragmentShader);
    if (!m_shader.isCompiled()){
        LOGE("Could not create program.\n");
        return false;
    }

    m_shader.use();

    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    printGLColorSpaceInfo();

    m_adjust.init();
    return true;
}


void MagicMain::renderFrame( float delta )
{
    if (!m_Engine) return;
    if (delta > MaxDeltaTime)
        delta = MaxDeltaTime;

    update(delta);
    //这个的坐标系和其他的稍有不同，所以这个放在前面执行，可以对其使用不同的Shader 
    if (m_Engine)
        m_Engine->drawImage();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0,0, m_ScreenWidth, m_ScreenHeight);

    drawImage();

    checkGlError("renderFrame");
}

void MagicMain::updateInputData( char* data, long len )
{
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        m_glYUVTex->uploadYUVTexImage(data);
    }else if(m_inputFortmat == IMAGE_FORMAT_RGB_565){
        m_SrcTex->uploadImageData((GLubyte*)data);
    }else if(m_inputFortmat == IMAGE_FORMAT_PACKET){
        m_SrcTex->loadFromMemory((unsigned char*)data, len);
    }
    m_Engine->updateInput(m_SrcTex);
}

void MagicMain::setInputDataInfo( int w, int h, int imageFormat )
{
    if (!m_SrcTex){
        m_SrcTex = new Texture;
        m_SrcTex->init();
    }
    m_SrcTex->setSize(w, h);
    m_inputFortmat = imageFormat;
    initEngine();

    //rgb565比rgb888快至少30%
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        m_glYUVTex = new glYUVTexture(w, h, m_SrcTex);
        m_glYUVTex->setImageAdjust(&m_adjust);
    }if(m_inputFortmat == IMAGE_FORMAT_RGB_565)
        m_SrcTex->setImageFormat(GDX2D_FORMAT_RGB565);
}


bool MagicMain::onTouchDown( float x, float y )
{
    x = transformX(x);
    y = transformY(y);
    if (m_Engine)
        m_Engine->onTouchDown(x, y);
    return true;
}

bool MagicMain::onTouchDrag( float x, float y )
{
    x = transformX(x);
    y = transformY(y);
    if (m_Engine)
        m_Engine->onTouchDrag(x, y);
    return true;
}

bool MagicMain::onTouchUp( float x, float y )
{
    x = transformX(x);
    y = transformY(y);
    if (m_Engine)
        m_Engine->onTouchUp(x, y);
    return true;
}

void MagicMain::setIOCallBack( FileUtils* callback )
{
    m_ioCallBack = callback;
    setFileUtils(callback);
}


void MagicMain::update( float delta )
{
    m_Engine->update(delta);
    if (m_nextEngine != EngineType_None && m_Engine->isFinished()){
        initEngine(m_nextEngine);
        m_nextEngine = EngineType_None;
    }
}

void MagicMain::drawImage()
{
    glDisable(GL_BLEND);
    m_outSprite.draw(&m_shader);
}


void MagicMain::setInputImage( const char* data, long len )
{
    if (!data || !len) return;
    if (!m_SrcTex){
        m_SrcTex = new Texture;
        m_SrcTex->init();
    }
    if (m_adjust.isNeedAdjust()){
        Texture tex ;
        tex.loadFromMemory((unsigned char*)data, len);
        m_adjust.apply(&tex, m_SrcTex);
    }else{
        m_SrcTex->loadFromMemory((unsigned char*)data, len);
    }
    if (!m_Engine)
        initEngine();
    m_Engine->setInputTexture(m_SrcTex);
}

void MagicMain::setInputImage( const char* imgPath )
{
    uint32_t size;
    unsigned char* buffer = EasyReadFile(imgPath, size);
    if (buffer && size > 0){
        setInputImage((const char*)buffer, size);
    }
    SafeDeleteArray(buffer);
}

void MagicMain::initEngine(EngineType type /*= EngineType_Mesh*/)
{
    SafeDelete(m_Engine);
    switch (type)
    {
    case EngineType_Effect:
        m_Engine = (MagicEngine* )(new EffectEngine());
    	break;
    default:
        m_Engine = (MagicEngine* )(new MeshEngine());
    }
    m_Engine->setMaxOutputSize(m_MaxWidth, m_MaxHeight);
    m_Engine->setPreviewSize(m_PreviewWidth, m_PreviewHeight);
    m_Engine->setOutputResize(this);
    m_Engine->SetIOCallBack(m_ioCallBack);
//    m_Engine->initEngine(m_adjust.getOutTexture());
    m_Engine->initEngine(m_SrcTex);
    m_Engine->start();
}

void MagicMain::switchEngine(EngineType type)
{
    if (m_Engine && m_Engine->type() == type) return;
    m_nextEngine = type;
    if (m_Engine)
        m_Engine->finish();
}

void MagicMain::takePicture( Texture *tex /*= NULL*/)
{
    if (!m_Engine) return;
    if (tex && m_adjust.isNeedAdjust()){
        Texture tmp;
        tmp.init();
        m_adjust.apply(m_SrcTex, &tmp);
        m_Engine->tackPicture(&tmp);
    }else{
        m_Engine->tackPicture(tex);
    }
    
}

void MagicMain::takePicture( const char* imgPath)
{
    uint32_t size;
    unsigned char* buffer = EasyReadFile(imgPath, size);
    if (buffer && size > 0){
        takePicture((const char*)buffer, size);
    }else{
        takePicture();
    }
    SafeDeleteArray(buffer);
}

void MagicMain::takePicture( const char* data , long len )
{
    if (data && len){
        Texture tex;
        tex.loadFromMemory((unsigned char*)data, len);
        takePicture(&tex);
    }else{
        takePicture();
    }

}

EngineType MagicMain::getEngineType()
{
    if (m_Engine)
        return m_Engine->type();
    else
        return EngineType_None;
}


// void MagicMain::setCover( const unsigned char* buffer, uint32_t size )
// {
//     if (!m_Engine || m_Engine->type() != EngineType_Effect)
//         return;
//     ((EffectEngine *)m_Engine)->setFrame(buffer, size);
// }

void MagicMain::setCover( const char* path )
{
    if (!m_Engine || m_Engine->type() != EngineType_Effect)
        return;
    ((EffectEngine *)m_Engine)->setCover(path);
}

void MagicMain::setEffect( const char* name )
{
    if (!m_Engine || m_Engine->type() != EngineType_Effect)
        return;
    ((EffectEngine *)m_Engine)->setEffect(name);
}

void MagicMain::setFrame( const char* path )
{
    if (!m_Engine || m_Engine->type() != EngineType_Effect)
        return;
    ((EffectEngine *)m_Engine)->setFrame(path);
}

void MagicMain::restoreMesh()
{
    if (m_Engine->type() != EngineType_Mesh)
        return;
    ((MeshEngine *)m_Engine)->restore();
}

void MagicMain::rotate90Input( bool clockwise /*= true*/)
{
    //TODO Fix：貌似坐标系有点问题！~ 上下翻转的 
    m_adjust.rotate90(!clockwise);
    if (!m_SrcTex) return;
    if (m_glYUVTex){
        m_glYUVTex->setImageAdjust(&m_adjust);   
    }else{
        Texture* tmp = new Texture();
        tmp->init();
        m_adjust.apply(m_SrcTex, tmp);
        SafeDelete(m_SrcTex);
        m_SrcTex = tmp;
    }
    m_Engine->setInputTexture(m_SrcTex);
}

void MagicMain::onEngineOutChange( Texture *tex )
{
    m_outSprite.setTexture(tex);
    //m_magicSprite.loadFromFile("assets/test2.jpg");
    //TODO 为什么需要flip？？？？！！！！ 
    m_outSprite.flip(false, true);
    m_outSprite.setScale(m_aspectRatio < m_outSprite.getAspect() ?
        m_aspectRatio/m_outSprite.getAspect() : 1.0);
}

void MagicMain::resize( int w, int h )
{
    LOGI("Resize(%d, %d)\n", w, h);
    m_aspectRatio = (float)w/h;
    m_ScreenWidth = w;
    m_ScreenHeight = h;
    setPreviewSize(w, h);
    m_shader.ortho(-m_aspectRatio/2, m_aspectRatio/2, -0.5, 0.5,-10, 10);
    m_outSprite.setScale(m_aspectRatio < m_outSprite.getAspect() ?
        m_aspectRatio/m_outSprite.getAspect() : 1.0);
}

void MagicMain::setPreviewSize( GLuint w, GLuint h )
{
    m_PreviewWidth = w;
    m_PreviewHeight = h;
}

const char* MagicMain::getEffectName()
{
    if (!m_Engine || m_Engine->type() != EngineType_Effect)
        return NULL;
    return ((EffectEngine *)m_Engine)->getEffectName();
}

void MagicMain::setParameter( const char* parameterKey, float value )
{
    if (!m_Engine || m_Engine->type() != EngineType_Effect)
        return;
    ((EffectEngine *)m_Engine)->setParameter(parameterKey, value);
}

float MagicMain::getParameterValue( const char* parameterKey )
{
    if (!m_Engine || m_Engine->type() != EngineType_Effect)
        return 0;
    return ((EffectEngine *)m_Engine)->getParameterValue(parameterKey);
}

const char* MagicMain::getParameterKeys()
{
    if (!m_Engine || m_Engine->type() != EngineType_Effect)
        return NULL;
    return ((EffectEngine *)m_Engine)->getParameterKeys();
}

const char* MagicMain::getEffectList()
{
    return ::getEffectList();
}

void MagicMain::setMaxOutputSize( int w, int h )
{
    GLuint dims[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, (GLint*)dims);
    m_MaxWidth = w > dims[0] ? dims[0] : w;
    m_MaxHeight = h > dims[1] ? dims[1] : h;
    if(m_Engine){
        m_Engine->setMaxOutputSize(m_MaxWidth, m_MaxHeight);
    }
}