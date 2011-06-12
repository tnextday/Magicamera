#include "framebufferobject.h"
#include "glutils.h"

FramebufferObject::FramebufferObject(void)
{
    glGenFramebuffers(1, m_fboId);
}

FramebufferObject::~FramebufferObject(void)
{
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
//     GLuint depthbuffer;
//     GLuint renderbuffers[1];
//     glGenRenderbuffers(1, renderbuffers);
//     depthbuffer = renderbuffers[0];
// 
//     glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
//     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 640, 480);
//     glFramebufferRenderbuffer(GL_FRAMEBUFFER,
//         GL_DEPTH_ATTACHMENT,
//         GL_RENDERBUFFER, depthbuffer);

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