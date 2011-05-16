
#include "Texture.h"



Texture::Texture()
{
	init();
}

// int getGLInternalFormat() {
// 	switch(format) {
// 		case GDX2D_FORMAT_ALPHA:
// 			return GL_ALPHA;
// 		case GDX2D_FORMAT_LUMINANCE_ALPHA:
// 			return GL_LUMINANCE_ALPHA;
// 		case GDX2D_FORMAT_RGB888:
// 		case GDX2D_FORMAT_RGB565:
// 			return GL_RGB;
// 		case GDX2D_FORMAT_RGBA8888:
// 		case GDX2D_FORMAT_RGBA4444:
// 			return GL_RGBA;
// 		default:
// 			//throw new GdxRuntimeException("unknown format: " + format);
// 	}
// }	


Texture::Texture( const unsigned char *buffer, uint32_t len, uint32_t req_format /*= GDX2D_FORMAT_RGBA8888*/)
{
	init();
	gdx2d_pixmap* pixmap = gdx2d_load(buffer, len, req_format);
	 GLenum dataFormat;
	switch(pixmap->format){
		case GDX2D_FORMAT_ALPHA:
			dataFormat = GL_ALPHA;
			break;
		case GDX2D_FORMAT_RGBA8888:
		case GDX2D_FORMAT_RGBA4444:
			dataFormat = GL_RGBA;
			break;
		case GDX2D_FORMAT_RGB565:
			dataFormat = GL_RGB565;
			break;
		default:
			dataFormat = GL_RGBA;
	}
	uploadImageData(pixmap->pixels, pixmap->width, pixmap->height, dataFormat);
}

Texture::Texture( char* texFilePath )
{
	init();
	//TODO load image from file
}

Texture::~Texture()
{
	GLuint texID[1];
	texID[0] = m_TexHandle;
	glDeleteTextures(1, m_TexHandle);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_TexHandle);
}

void Texture::bind( int unit )
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_TexHandle);
}

void Texture::setWrap( GLfloat wrap_s /*= GL_REPEAT*/, GLfloat wrap_t /*= GL_REPEAT*/ )
{
	bind();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t); 
}

void Texture::setFilter( GLfloat min /*= GL_LINEAR*/, GLfloat mag /*= GL_LINEAR*/ )
{
	bind();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture::uploadImageData( char* data, int width, int height, GLenum dataFormat )
{
	bind();
	m_Width = width;
	m_Height = height;
	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
}

GLuint Texture::createGLHandle()
{
	GLuint texID[1];
	glGenTextures(1, texID);
	m_TexHandle = texID[0];
}

void Texture::init()
{
	createGLHandle();
	setWrap();
	setFilter();
}