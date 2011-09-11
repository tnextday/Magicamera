#include "blureffect.h"

BlurEffect::BlurEffect(void)
{
}

BlurEffect::~BlurEffect(void)
{
}

void BlurEffect::onInit()
{
    uint32_t size = 0;
    unsigned char* sbuf = readRes("shaders/blur.sp", size);
    if (sbuf && size > 0){
        m_shader.loadFromMemory((char* )sbuf, size);
    }
    delete [] sbuf;
    m_shader.use();
    m_imgTexLoc = glGetUniformLocation(m_shader.getProgram(), "uImgTex");
    m_stepXLoc = glGetUniformLocation(m_shader.getProgram(), "g_fStepSizeX");
    m_stepYLoc = glGetUniformLocation(m_shader.getProgram(), "g_fStepSizeY");
    m_stepSize = 1;
}

void BlurEffect::onResize()
{
    m_stepX = m_stepSize/m_width;
    m_stepY = m_stepSize/m_height;
}

void BlurEffect::setStepSize( float ss )
{
    m_stepSize = ss;
    onResize();
}

void BlurEffect::onDoEffect()
{
    glUniform1f(m_stepXLoc, m_stepX);
    glUniform1f(m_stepYLoc, m_stepY);
}