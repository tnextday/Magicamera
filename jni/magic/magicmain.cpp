#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "magicmain.h"
#include "glutils/glutils.h"
#include "utils/mathelpers.h"
#include "coverengine.h"

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
}

MagicMain::~MagicMain()
{
    SafeDelete(m_glYUVTex);
    SafeDelete(m_Engine);
}


bool MagicMain::setupGraphics() {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    m_shader.makeProgram(gVertexShader, gFragmentShader);
    if (!m_shader.isCompiled()){
        LOGE("Could not create program.\n");
        return false;
    }

    m_shader.use();

    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    //启用混合操作
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_SrcTex.init();

    printGLColorSpaceInfo();

    glEnableVertexAttribArray(m_shader.getPositionLoc());
    glEnableVertexAttribArray(m_shader.getTextureCoordLoc());

//     m_adjust.init();
//     m_adjust.setOnOutputChange(this);

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
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0,0, m_ScreenWidth, m_ScreenHeight);

    drawImage();

    checkGlError("renderFrame");
}

void MagicMain::updatePreviewData( char* data, long len )
{
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        //m_PreviewTex->uploadImageData((GLubyte*)m_tmpImageData);
        m_glYUVTex->uploadYUVTexImage(data, m_SrcTex.getWidth(), m_SrcTex.getHeight());
    }else if(m_inputFortmat == IMAGE_FORMAT_RGB_565){
        m_SrcTex.uploadImageData((GLubyte*)data);
    }else if(m_inputFortmat == IMAGE_FORMAT_PACKET){
        m_SrcTex.loadFromMemory((unsigned char*)data, len);
    }
/*    m_adjust.drawImage();*/
    m_Engine->updateInput(&m_SrcTex);
}

void MagicMain::setPreviewDataInfo( int w, int h, int imageFormat )
{
    m_SrcTex.setSize(w, h);
    m_inputFortmat = imageFormat;
/*    m_adjust.setInputTexture(&m_SrcTex);*/
    initEngine();

    //rgb565比rgb888快至少30%
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        m_glYUVTex = new glYUVTexture(w, h, m_SrcTex.getTexHandle());
    }if(m_inputFortmat == IMAGE_FORMAT_RGB_565)
        m_SrcTex.setImageFormat(GDX2D_FORMAT_RGB565);

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
    m_shader.use();
//    m_SrcTex.bind();
    glDisable(GL_BLEND);

    m_magicSprite.draw(&m_shader);
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
}


void MagicMain::setPreviewImage( const char* data, long len )
{
    m_SrcTex.loadFromMemory((unsigned char*)data, len);
//     m_adjust.setInputTexture(&m_SrcTex);
//     m_adjust.drawImage();
    initEngine();
}

void MagicMain::setPreviewImage( const char* imgPath )
{
    m_SrcTex.loadFromFile(imgPath);
//     m_adjust.setInputTexture(&m_SrcTex);
//     m_adjust.drawImage();
    initEngine();
}

void MagicMain::initEngine(EngineType type /*= EngineType_Mesh*/)
{
    SafeDelete(m_Engine);
    switch (type)
    {
    case EngineType_Cover:
        m_Engine = (MagicEngine* )(new CoverEngine());
    	break;
    default:
        m_Engine = (MagicEngine* )(new MeshEngine());
    }

    m_Engine->setOutputResize(this);
    m_Engine->SetIOCallBack(m_ioCallBack);
//    m_Engine->initEngine(m_adjust.getOutTexture());
    m_Engine->initEngine(&m_SrcTex);
    m_Engine->start();
}

void MagicMain::switchEngine(EngineType type)
{
    if (m_Engine && m_Engine->type() == type) return;
    m_nextEngine = type;
    if (m_Engine)
        m_Engine->finish();
}

void MagicMain::takePicture()
{
    if (m_Engine)
        m_Engine->tackPicture();
}

EngineType MagicMain::getEngineType()
{
    if (m_Engine)
        return m_Engine->type();
    else
        return EngineType_None;
}


void MagicMain::setCover( const unsigned char* buffer, uint32_t size )
{
    if (!m_Engine || m_Engine->type() != EngineType_Cover)
        return;
    ((CoverEngine *)m_Engine)->setCover(buffer, size);
}

void MagicMain::setCover( const char* path )
{
    if (!m_Engine || m_Engine->type() != EngineType_Cover)
        return;
    ((CoverEngine *)m_Engine)->setCover(path);
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
//     m_adjust.rotate90(!clockwise);
//     m_adjust.drawImage();
}

void MagicMain::onEngineOutChange( Texture *tex )
{
    m_magicSprite.setTexture(tex);
    //m_magicSprite.loadFromFile("assets/test2.jpg");
    //TODO 为什么需要flip？？？？！！！！
    m_magicSprite.flip(false, true);
}

void MagicMain::onAdjustChange( Texture *tex )
{
    if (m_Engine)
        m_Engine->setInputTexture(tex);
}

void MagicMain::resize( int w, int h )
{
    LOGI("Resize(%d, %d)\n", w, h);
    m_aspectRatio = (float)w/h;
    m_ScreenWidth = w;
    m_ScreenHeight = h;
    m_shader.ortho(-m_aspectRatio/2, m_aspectRatio/2, -0.5, 0.5,-10, 10);
}

void MagicMain::setEffect( const char* path )
{
    if (!m_Engine || m_Engine->type() != EngineType_Cover)
        return;
    ((CoverEngine *)m_Engine)->setEffect(path);
}