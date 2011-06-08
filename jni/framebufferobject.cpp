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
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
	unbind();
}

bool FramebufferObject::check_status()
{
	return glCheckFramebufferStatus(m_fboId[0]) == GL_FRAMEBUFFER_COMPLETE;
}