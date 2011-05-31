
#include "texture.h"
#include "glutils.h"


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
	uploadImageData(buffer, len, req_format);
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
	glDeleteTextures(1, texID);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_TexHandle);
	checkGlError("glBindTexture");
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

void Texture::uploadImageData( GLubyte* data, int width, int height, GLenum imageFormat )
{
	bind();
	m_Width = width;
	m_Height = height;
	m_imageFormat = imageFormat;
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	checkGlError("uploadImageData");
}

void Texture::uploadImageData( GLubyte* data )
{
	uploadImageData(data, m_Width, m_Height, m_imageFormat);
}

void Texture::uploadImageData( const unsigned char *buffer, uint32_t len, uint32_t req_format /*= GDX2D_FORMAT_RGBA8888*/ )
{
	gdx2d_pixmap* pixmap = gdx2d_load(buffer, len, req_format);
	switch(pixmap->format){
		case GDX2D_FORMAT_ALPHA:
			m_imageFormat = GL_ALPHA;
			break;
		case GDX2D_FORMAT_RGBA8888:
		case GDX2D_FORMAT_RGBA4444:
			m_imageFormat = GL_RGBA;
			break;
		case GDX2D_FORMAT_RGB565:
			m_imageFormat = GL_RGB565;
			break;
		default:
			m_imageFormat = GL_RGBA;
	}
	m_Width = pixmap->width;
	m_Height = pixmap->height;
	uploadImageData((GLubyte*)(pixmap->pixels), pixmap->width, pixmap->height, m_imageFormat);
	gdx2d_free(pixmap);
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
