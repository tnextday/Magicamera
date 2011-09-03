#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "magicengine.h"
#include "glutils/glutils.h"
#include "utils/mathelpers.h"

MagicEngine::MagicEngine()
{
    m_InTex = NULL;
    m_shader = NULL;
    m_fbo = NULL;
    m_sfactor = GL_SRC_ALPHA;
    m_dfactor = GL_ONE_MINUS_SRC_ALPHA;
    m_ioCallBack = NULL;
    m_onOutputResize = NULL;
}

MagicEngine::MagicEngine( BaseShader* shader, Texture* SrcTex)
{
    m_sfactor = GL_SRC_ALPHA;
    m_dfactor = GL_ONE_MINUS_SRC_ALPHA;
    m_ioCallBack = NULL;
    m_onOutputResize = NULL;
    m_InTex = NULL;
    initEngine(shader, SrcTex);
}

MagicEngine::~MagicEngine()
{
    SafeDelete(m_fbo);
}

bool MagicEngine::initEngine(BaseShader* shader, Texture* SrcTex) {
    m_OutTex.init();
    setShader(shader);
    setInputTexture(SrcTex);
    m_fbo = new FramebufferObject();
    m_fbo->texture2d(m_OutTex.getTexHandle());

    onInit();
    return true;
}


void MagicEngine::drawImage()
{
    m_fbo->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    draw();
    m_fbo->unbind();
}

//设置输出图片大小，不同于坐标
void MagicEngine::setSize( int w, int h )
{
    //输出图片的比例和坐标系相同 
    m_width = w;
    m_height = w*m_coordHeight/m_coordWidth;
    m_OutTex.setSize(m_width, m_height);
}


//保存图片会以输入图片的大小为准，但是会切割成处理后大小的比例
void MagicEngine::tackPicture(Texture *texutre /*= NULL*/)
{
    int ow = m_width, oh = m_height;
    m_fbo->bind();
    if (texutre){
        setSize(texutre->getWidth(), texutre->getHeight());
        draw(texutre);
    }else{
        setSize(m_InTex->getWidth(), m_InTex->getHeight());
        draw();
    }

    int size = m_width*m_height*4;
    char* pixels = new char[size];

    glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    if(m_ioCallBack){
        m_ioCallBack->SaveImage((char*)pixels, m_width, m_height, 0);
    }
    delete[] pixels;
    checkGlError("SavePicture");
    m_fbo->unbind();

    setSize(ow, oh);
}

void MagicEngine::tackPicture( const char* data, long len )
{
    Texture tex;
    tex.loadFromMemory((unsigned char*)data, len);
    tackPicture(&tex);
}

void MagicEngine::tackPicture( const char* imagePath )
{
    Texture tex;
    tex.loadFromFile(imagePath);
    tackPicture(&tex);
}

void MagicEngine::tackPicture( const char* data, int w, int h, int format )
{
    //TODO 暂时用不到，有时间完善 
}

void MagicEngine::setBlendFunc( GLenum sfactor, GLenum dfactor )
{
    m_sfactor = sfactor;
    m_dfactor = dfactor;
}


bool MagicEngine::onTouchDown( float x, float y )
{
    return false;
}

bool MagicEngine::onTouchDrag( float x, float y )
{
    return false;
}

bool MagicEngine::onTouchUp( float x, float y )
{
    return false;
}

void MagicEngine::draw( Texture *texutre /*= NULL*/ )
{
    glViewport(0,0, m_width, m_height);
    m_shader->use();
    m_shader->setViewProj(m_vp);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(m_shader->getPositionLoc());
    glEnableVertexAttribArray(m_shader->getTextureCoordLoc());
    onDraw(texutre);
}

void MagicEngine::SetIOCallBack( IOCallBack* val )
{
    m_ioCallBack = val;
}

void MagicEngine::resizeCoord()
{
    m_coordWidth = g_CoordWidth;
    m_coordHeight = m_coordWidth*m_InTex->getHeight()/m_InTex->getWidth();
    setSize(m_coordWidth, m_coordHeight);
    //重置坐标系
    matIdentity(m_vp);
    matOrtho(m_vp, 0, m_coordWidth, 0, m_coordHeight, -10, 10);
    if (m_onOutputResize){
        m_onOutputResize->onEngineOutChange(&m_OutTex);
    }
}

void MagicEngine::setInputTexture( Texture* val )
{
    if (!val) return;
    m_InTex = val;
    resizeCoord();
}

