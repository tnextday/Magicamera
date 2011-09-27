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
    m_fbo = NULL;
    m_ioCallBack = NULL;
    m_PreviewWidth = 640;
    m_PreviewHeight = 480;
    m_onOutputResize = NULL;
}

// MagicEngine::MagicEngine(Texture* SrcTex)
// {
//     m_ioCallBack = NULL;
//     m_onOutputResize = NULL;
//     m_InTex = NULL;
//     m_PreviewWidth = 640;
//     m_PreviewHeight = 480;
//     initEngine(SrcTex);
// }

MagicEngine::~MagicEngine()
{
    SafeDelete(m_fbo);
}

bool MagicEngine::initEngine(Texture* SrcTex) {
    m_OutTex.init();
    m_shader.loadFromFile("res://shaders/default.sp");
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




//保存图片会以输入图片的大小为准，但是会切割成处理后大小的比例
void MagicEngine::tackPicture(Texture *texutre /*= NULL*/)
{
    int ow = m_width, oh = m_height;
    m_fbo->bind();
    if (texutre){
        setSize(texutre->getWidth(), texutre->getHeight(), false);
    }else{
        setSize(m_InTex->getWidth(), m_InTex->getHeight(), false);
    }
    draw(texutre);

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
    tex.init();
    tex.loadFromMemory((unsigned char*)data, len);
    tackPicture(&tex);
}

void MagicEngine::tackPicture( const char* imagePath )
{
    Texture tex;
    tex.init();
    tex.loadFromFile(imagePath);
    tackPicture(&tex);
}

void MagicEngine::tackPicture( const char* data, int w, int h, int format )
{
    //TODO 暂时用不到，有时间完善 
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
    m_shader.use();
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(m_shader.getPositionLoc());
    glEnableVertexAttribArray(m_shader.getTextureCoordLoc());
    onDraw(texutre);
}

void MagicEngine::SetIOCallBack( FileUtils* val )
{
    m_ioCallBack = val;
}

void MagicEngine::resizeCoord( int w, int h )
{
    m_aspectRatio = (float)w/h;
    m_shader.ortho(-m_aspectRatio/2, m_aspectRatio/2, -0.5, 0.5, -10, 10);
    setSize(w, h);
}

//设置输出图片大小，不同于坐标
void MagicEngine::setSize( int w, int h , bool bPreview /*= true*/)
{
    m_width = w;
    m_height = h;
    if (bPreview){
        float aspect = (float)w/h;
        if (aspect > m_aspectRatio && m_width > m_PreviewWidth){
            m_width = m_PreviewWidth;
            m_height = m_width/aspect;
        }else if (m_height > m_PreviewHeight){
            m_height = m_PreviewHeight;
            m_width = m_height*aspect;
        }
    }
    m_OutTex.setSize(m_width, m_height);
    if (m_onOutputResize){
        m_onOutputResize->onEngineOutChange(&m_OutTex);
    }
}

void MagicEngine::setInputTexture( Texture* val )
{
    if (!val) return;
    m_InTex = val;
    if (m_width != val->getWidth() || m_height != val->getHeight())
        resizeCoord(val->getWidth(), val->getHeight());
}

void MagicEngine::setPreviewSize( GLuint w, GLuint h )
{
    m_PreviewWidth = w;
    m_PreviewHeight = h;
}

