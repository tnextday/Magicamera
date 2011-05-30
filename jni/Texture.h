#ifndef _Texture_h_
#define _Texture_h_

#include <GLES2/gl2.h>
#include "gdx2d/gdx2d.h"



class Texture
{
	GLuint m_TexHandle;
	GLuint m_Width;
	GLuint m_Height;
	GLenum m_imageFormat;

public:
	Texture();

	//************************************
	// Method: 内存解码并生成texture
	// Description: 
	// Parameter: const unsigned char * buffer
	// Parameter: uint32_t len
	// Parameter: uint32_t req_format
	// Returns:   
	//************************************
	Texture(const unsigned char *buffer, uint32_t len, uint32_t req_format = GDX2D_FORMAT_RGBA8888);
	Texture(char* texFilePath);
	~Texture();

	void bind();
	void bind(int unit);
	void setWrap(GLfloat wrap_s = GL_REPEAT, GLfloat wrap_t = GL_REPEAT);
	void setFilter(GLfloat min = GL_LINEAR, GLfloat mag = GL_LINEAR);
	void uploadImageData(GLubyte* data, int width, int height, GLenum imageFormat);
	void uploadImageData(GLubyte* data);
	void setSize(int w, int h){m_Width = w; m_Height = h;};
	//GL Format
	void setImageFormat(GLenum imageFormat){ m_imageFormat = imageFormat;};
	GLuint createGLHandle();

private:
	void init();
};



#endif // _Texture_h_