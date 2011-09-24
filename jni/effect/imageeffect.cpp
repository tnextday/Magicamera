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
    m_fbo = new FramebufferObject();
}

ImageEffect::ImageEffect( const char* fileName )
{
    m_texWidthLoc = -1;
    m_texHeightLoc = -1;
    m_fbo = new FramebufferObject();
    loadFromFile(fileName);
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

bool ImageEffect::loadFromFile( const char* fileName )
{
    bool result = true;
    uint32_t size = 0;
    unsigned char* sbuf = EasyReadFile(fileName, size);
    if (sbuf && size > 0){
        result = loadFromMemory((char* )sbuf, size);
    }
    SafeDeleteArray(sbuf);
    return result;
}

void ImageEffect::process( Texture* src, Texture* dst )
{
    GLfloat w,h;
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
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1); //话说不关掉这个就会报错
    glVertexAttribPointer(m_shader.getPositionLoc(), 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_fbo->unbind();
    checkGlError("ImageEffect::process");
}