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
    m_texWidthLoc = -1;
    m_texHeightLoc = -1;
}

ImageEffect::~ImageEffect(void)
{
    SafeDelete(m_fbo);
}

bool ImageEffect::loadFromMemory( const char* buf, int size )
{
    bool bRet = m_shader.loadFromMemory(buf,size);
    if (bRet){
        m_shader.use();
        m_imgTexLoc = glGetUniformLocation(m_shader.getProgram(), "uImgTex");
        m_texHeightLoc = glGetUniformLocation(m_shader.getProgram(), "uTexHeight");
        m_texWidthLoc = glGetUniformLocation(m_shader.getProgram(), "uTexWidth");
    }
    return bRet;
}

bool ImageEffect::loadFromRes( const char* fileName )
{
    bool result = true;
    uint32_t size = 0;
    unsigned char* sbuf = readRes(fileName, size);
    if (sbuf && size > 0){
        result = m_shader.loadFromMemory((char* )sbuf, size);
    }
    SafeDeleteArray(sbuf);
    return result;
}

void ImageEffect::process()
{
    process(m_InTex, &m_OutTex);
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

void ImageEffect::process( Texture* src, Texture* dst )
{
    GLuint w,h;
    w = src->getWidth();
    h = src->getHeight();
    dst->setSize(w, h);
    m_shader.use();
    if (m_texWidthLoc >= 0)
        glUniform1f(m_texWidthLoc, w);
    if (m_texHeightLoc >= 0)
        glUniform1f(m_texHeightLoc, h);
    glDisable(GL_DEPTH_TEST);
    m_fbo->bindWithTexture(dst->getTexHandle()); 
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    src->bind();
    glUniform1i(m_imgTexLoc, 0);
    onProcess(src, dst);
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1); //话说不关掉这个就会报错
    glVertexAttribPointer(m_shader.getPositionLoc(), 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_fbo->unbind();
    checkGlError("ImageEffect::process");
}