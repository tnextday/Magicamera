#include "framebufferobject.h"
#include "glutils.h"

FramebufferObject::FramebufferObject(void)
{
    glGenFramebuffers(1, m_fboId);
    m_colorBuffer = 0;
    m_depthBuffer = 0;
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

void FramebufferObject::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId[0]);
}

void FramebufferObject::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void FramebufferObject::resizeColorBuffer( int w, int h )
{
    if (m_colorBuffer == 0){
        m_colorBuffer = genRenderbuffer();
    }


    bind();
    glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_RENDERBUFFER, m_colorBuffer);
    checkGlError("createColorBuffer");
    /*unbind();*/
}

void FramebufferObject::resizeDepthBuffer( int w, int h )
{
    if (m_depthBuffer == 0){
        m_depthBuffer = genRenderbuffer();
    }
    bind();
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, m_depthBuffer);
    checkGlError("createColorBuffer");
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