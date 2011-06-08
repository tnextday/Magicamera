
#ifndef _framebufferobject_h_
#define _framebufferobject_h_

#include <GLES2\gl2.h>

class FramebufferObject
{

private:
	GLuint m_fboId[1];

public:
	FramebufferObject(void);
	~FramebufferObject(void);

	void bind();
	void unbind();
	void texture2d(GLuint texId);
	bool check_status();
};
#endif // _framebufferobject_h_
