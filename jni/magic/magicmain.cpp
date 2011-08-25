#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "magicmain.h"
#include "glutils/glutils.h"
#include "utils/mathelpers.h"
#include "coverengine.h"

static const char gVertexShader[] = 
        "uniform mat4 uMVPMatrix;\n"
        "attribute vec4 aPosition;\n"
        "attribute vec2 aTextureCoord;\n"
        "varying vec2 vTextureCoord;\n"
        "void main() {\n"
        "  gl_Position = uMVPMatrix * aPosition;\n" 
        "  vTextureCoord = aTextureCoord;\n"
        "}\n";

static const char gFragmentShader[] = 
        "precision mediump float;\n"
        "varying vec2 vTextureCoord;\n"
        "uniform sampler2D sTexture;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
        "}\n";

MagicMain::MagicMain()
{
    m_glYUVTex = NULL;
    m_SrcTex = NULL;
    m_resPath[0] = '\0';
    m_saveImage = NULL;
    m_Engine = NULL;
    m_nextEngine = EngineType_None;
}

MagicMain::~MagicMain()
{
    SafeDelete(m_SrcTex);
    SafeDelete(m_glYUVTex);
    SafeDelete(m_Engine);
}


bool MagicMain::setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("\n");
    LOGI("setupGraphics(%d, %d)\n", w, h);
    m_ScreenWidth = w;
    m_ScreenHeight = h;
    m_CoordWidth = g_CoordWidth;
    m_CoordHeight = g_CoordWidth*h/w;

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

    m_SrcTex = new Texture();
    m_SrcTex->init();

    printGLColorSpaceInfo();
    matIdentity(m_vp);
    matOrtho(m_vp, 0, m_CoordWidth, 0, m_CoordHeight, -10, 10);
    glEnableVertexAttribArray(m_shader.getPositionLoc());
    glEnableVertexAttribArray(m_shader.getTextureCoordLoc());

    return true;
}


void MagicMain::renderFrame( float delta )
{
    update(delta);
    //这个的坐标系和其他的稍有不同，所以这个放在前面执行，可以对其使用不同的Shader
    m_Engine->drawImage();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0,0, m_ScreenWidth, m_ScreenHeight);
    m_shader.setViewProj(m_vp);

    drawImage();
    drawUI();
    checkGlError("renderFrame");
}

void MagicMain::updatePreviewData( char* data, long len )
{
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        //m_PreviewTex->uploadImageData((GLubyte*)m_tmpImageData);
        m_glYUVTex->uploadYUVTexImage(data, m_SrcTex->getWidth(), m_SrcTex->getHeight());
    }else if(m_inputFortmat == IMAGE_FORMAT_RGB_565){
        m_SrcTex->uploadImageData((GLubyte*)data);
    }else if(m_inputFortmat == IMAGE_FORMAT_PACKET){
        m_SrcTex->uploadImageData((unsigned char*)data, len);
    }
}

void MagicMain::setPreviewDataInfo( int w, int h, int imageFormat )
{
    m_SrcTex->setSize(w, h);
    m_inputFortmat = imageFormat;

    initEngine();

    //rgb565比rgb888快至少30%
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        m_glYUVTex = new glYUVTexture(w, h, m_SrcTex->getTexHandle());
    }if(m_inputFortmat == IMAGE_FORMAT_RGB_565)
        m_SrcTex->setImageFormat(GDX2D_FORMAT_RGB565);
}


bool MagicMain::onTouchDown( float x, float y )
{
    x = x*m_CoordWidth/m_ScreenWidth;
    y = y*m_CoordHeight/m_ScreenHeight;
    //LOGI("onTouchDown: %.1f, %.1f\n", x, y);
    y = m_CoordHeight - y;
    m_btn_func->onTouchDown(x, y);
    m_btn_switch->onTouchDown(x, y);
    m_BtnSave->onTouchDown(x, y);
    m_Engine->onTouchDown(x, y - m_magicSpriteY);
    return true;
}

bool MagicMain::onTouchDrag( float x, float y )
{
    x = x*m_CoordWidth/m_ScreenWidth;
    y = y*m_CoordHeight/m_ScreenHeight;
    //LOGI("onTouchDrag: %.1f, %.1f\n", x, y);
    y = m_CoordHeight - y;
    m_Engine->onTouchDrag(x, y - m_magicSpriteY);
    return true;
}

bool MagicMain::onTouchUp( float x, float y )
{
    x = x*m_CoordWidth/m_ScreenWidth;
    y = y*m_CoordHeight/m_ScreenHeight;
    //LOGI("onTouchUp: %.1f, %.1f\n", x, y);
    y = m_CoordHeight - y;
    m_btn_func->onTouchUp(x, y);
    m_BtnSave->onTouchUp(x, y);
    m_btn_switch->onTouchUp(x, y);
    m_Engine->onTouchUp(x, y - m_magicSpriteY);
    return true;
}

void MagicMain::setCallBack( SaveImageCallBack* callback )
{
    m_saveImage = callback;
}


void MagicMain::update( float delta )
{
    m_Engine->update(delta);
    if (m_nextEngine != EngineType_None && m_Engine->isFinished()){
        initEngine(m_nextEngine);
        m_nextEngine = EngineType_None;
    }
}

void MagicMain::drawUI()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_btn_func->draw(&m_shader);
    m_BtnSave->draw(&m_shader);
    m_btn_switch->draw(&m_shader);
}

void MagicMain::drawImage()
{
    m_shader.use();
    m_SrcTex->bind();
    glDisable(GL_BLEND);
    m_magicSprite.draw(&m_shader);
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
}

void MagicMain::setResPath(const char* path )
{
    snprintf(m_resPath, _MAX_PATH-1, "%s", path);
    LOGI("setSaveImagePath : %s\n", path);
}

void MagicMain::loadRes()
{
    char path[_MAX_PATH];
    char path1[_MAX_PATH];

    m_BtnSave = new Button(makeResPath(path, "ui/btn_bg.png"), 
                           makeResPath(path1, "ui/btn_img_save.png"), 1);
    m_BtnSave->setOnClick(this);
    m_BtnSave->setPostion(m_BtnSave->getRegionWidth()/2, m_BtnSave->getRegionHeight()/2);

    m_btn_switch = new Button(makeResPath(path, "ui/btn_bg.png"), NULL, 2);
    m_btn_switch->setOnClick(this);
    m_btn_switch->setPostion(m_CoordWidth/2, m_btn_switch->getRegionHeight()/2);

    m_btn_func = new Button(makeResPath(path, "ui/btn_bg.png"), NULL, 3);
    m_btn_func->setOnClick(this);
    m_btn_func->setPostion(m_CoordWidth -  m_btn_func->getRegionWidth()/2, m_btn_func->getRegionHeight()/2);
}

char* MagicMain::makeResPath( char* path, const char* targetFile, int szBuffer/* = _MAX_PATH*/)
{
    snprintf(path, szBuffer, "%s/%s", m_resPath, targetFile);
    return path;
}

void MagicMain::onButtonClick( Button *btn )
{
    char path[_MAX_PATH];
    if (btn->tag() == 3) {
        if (m_Engine->type() == EngineType_Mesh)
            ((MeshEngine *)m_Engine)->restore();
        else if (m_Engine->type() == EngineType_Cover){
            srand(time(0));
            snprintf(path, _MAX_PATH-1, "./assets/frame/%02d.png", rand()%4+1);
            ((CoverEngine *)m_Engine)->setCover(path);
        }
    } else if (btn->tag() == 2){
        EngineType type;
        if (m_Engine->type() == EngineType_Mesh)
            type = EngineType_Cover;
        else 
            type = EngineType_Mesh;
        switchEngine(type);
    } else if (btn->tag() == 1)
        m_Engine->tackPicture();
    LOGI("onButtonClick : %d\n", btn->tag());
}

void MagicMain::setPreviewImage( const char* data, long len )
{
    m_SrcTex->uploadImageData((unsigned char*)data, len);
    initEngine();
}

void MagicMain::setPreviewImage( const char* imgPath )
{
    m_SrcTex->loadFromFile(imgPath);
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

    m_Engine->initEngine(&m_shader, m_SrcTex);
    m_magicSprite.setTexture(m_Engine->getOutTexture());
    m_magicSprite.setPostion(m_CoordWidth/2, m_CoordHeight/2);
    m_magicSpriteY = (m_CoordHeight - m_magicSprite.getRegionHeight())/2;
    //TODO 为什么需要flip？？？？！！！！
    m_magicSprite.flip(false, true);
    m_Engine->SetSaveImageCallBack(m_saveImage);
    m_Engine->start();
}

void MagicMain::OnOutputSizeChange( int w, int h )
{
    m_magicSpriteY = (m_CoordHeight - h)/2;
}

void MagicMain::switchEngine(EngineType type)
{
    if (m_Engine->type() == type) return;
    m_nextEngine = type;
    m_Engine->finish();
}

