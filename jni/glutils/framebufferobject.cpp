#include "framebufferobject.h"
#include "glutils.h"

FramebufferObject::FramebufferObject(bool bColorBUffer /*= false*/, bool bDepthBuffer/* = false*/)
{
    glGenFramebuffers(1, m_fboId);
    m_colorBuffer = 0;
    m_depthBuffer = 0;
    m_height = 0;
    m_width = 0;
    m_bUseColorBuffer = bColorBUffer;
    m_bUseDepthBuffer = bDepthBuffer;
}

FramebufferObject::~FramebufferObject(void)
{
    if (m_colorBuffer != 0){
        deleteRenderBuffer(m_colorBuffer);
        m_colorBuffer = 0;
    }
    if (m_depthBuffer != 0){
        deleteRenderBuffer(m_depthBuffer);
        m_colorBuffer = 0;
    }
    glDeleteFramebuffers(1, m_fboId);
}


void FramebufferObject::texture2d( GLuint texId )
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    check_status();
    unbind();
}

bool FramebufferObject::check_status()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    checkGlError("glCheckFramebufferStatus");
    switch (status){
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOGE("FramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOGE("FramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            LOGE("FramebufferStatus: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOGE("FramebufferStatus: GL_FRAMEBUFFER_UNSUPPORTED\n");
            break;
    }
    return status == GL_FRAMEBUFFER_COMPLETE;
}

void FramebufferObject::createColorBuffer()
{
    if (m_colorBuffer != 0){
        deleteRenderBuffer(m_colorBuffer);   
    }
    m_colorBuffer = genRenderbuffer();
    glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB5_A1, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_RENDERBUFFER, m_colorBuffer);
    checkGlError("createColorBuffer");
    /*unbind();*/
}

void FramebufferObject::createDepthBuffer()
{
    if (m_depthBuffer != 0){
        deleteRenderBuffer(m_depthBuffer);   
    }
    m_depthBuffer = genRenderbuffer();
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, m_depthBuffer);
    checkGlError("createDepthBuffer");
    /*unbind();*/
}

GLuint FramebufferObject::genRenderbuffer()
{
    GLuint renderbuffers[1];
    glGenRenderbuffers(1, renderbuffers);
    return renderbuffers[0];
}

void FramebufferObject::deleteRenderBuffer( GLuint bufferId )
{
    GLuint renderbuffers[1];
    renderbuffers[0] = bufferId;
    glDeleteRenderbuffers(1, renderbuffers);
}

void FramebufferObject::resizeBuffers( int w, int h )
{
    if(m_width == w && m_height == h) return;
    m_width = w;
    m_height = h;
    bind();
    if (m_bUseColorBuffer)
        createColorBuffer();
    if (m_bUseDepthBuffer)
        createDepthBuffer();
    unbind();
}

void FramebufferObject::bindWithTexture( GLuint texId )
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
}