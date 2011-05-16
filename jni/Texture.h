#ifndef _Texture_h_
#define _Texture_h_

#include <GLES2/gl2.h>
#include "gdx2d/gdx2d.h"

class Texture
{
	GLuint m_TexHandle;
	GLuint m_Width;
	GLuint m_Height;

public:
	Texture();
	Texture(const unsigned char *buffer, uint32_t len, uint32_t req_format = GDX2D_FORMAT_RGBA8888);
	Texture(char* texFilePath);
	~Texture();

	void bind();
	void bind(int unit);
	void setWrap(GLfloat wrap_s = GL_REPEAT, GLfloat wrap_t = GL_REPEAT);
	void setFilter(GLfloat min = GL_LINEAR, GLfloat mag = GL_LINEAR);
	void uploadImageData(char* data, int width, int height, GLenum dataFormat);
	GLuint createGLHandle();

private:
	void init();
};

#endif // _Texture_h_