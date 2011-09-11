#include "imageeffect.h"
#include <zlib.h>
#include <stdio.h>

static const GLfloat G_QuadData[] = {
    // X, Y, U, V
    -1.0f, -1.0f, 0.0f, 1.0f,
    +1.0f, -1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, 1.0f, 0.0f,
    -1.0f, +1.0f, 0.0f, 0.0f,
};

ImageEffect::ImageEffect(void)
{
}

ImageEffect::~ImageEffect(void)
{
    SafeDelete(m_fbo);
}

bool ImageEffect::loadFromMemory( const char* buf, int size )
{
    return m_shader.loadFromMemory(buf,size);
}

bool ImageEffect::loadFromRes( const char* fileName )
{
    bool result = true;
    return result;
}

void ImageEffect::doEffect()
{
    m_shader.use();
    glDisable(GL_DEPTH_TEST);
    m_fbo->bind();
    //printGLColorSpaceInfo();
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //m_InTex->bind(1);
    glUniform1i(m_imgTexLoc, 1);

    onDoEffect();

    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1); //话说不关掉这个就会报错
    glVertexAttribPointer(m_shader.getPositionLoc(), 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_fbo->unbind();
    checkGlError("ImageEffect::doEffect");
}

void ImageEffect::setInputTex( Texture * tex )
{
    m_InTex = tex;
    m_width = m_InTex->getWidth();
    m_height = m_InTex->getHeight();
    m_OutTex.setSize(m_width, m_height);
    onResize();
}

void ImageEffect::Init()
{
    m_width = 0;
    m_height = 0;
    m_OutTex.init();
    m_fbo = new FramebufferObject();
    m_fbo->texture2d(m_OutTex.getTexHandle());
    onInit();
}