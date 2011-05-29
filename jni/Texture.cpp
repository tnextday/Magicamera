
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
	uploadImageData((GLubyte*)(pixmap->pixels), pixmap->width, pixmap->height, dataFormat);
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

inline void decodeYUV420SP(int* rgb, char* yuv420sp, int width, int height) 
{
	int frameSize = width * height;

	for (int j = 0, yp = 0; j < height; j++) {
		int uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
		for (int i = 0; i < width; i++, yp++) {
			int y = (0xff & ((int) yuv420sp[yp])) - 16;
			if (y < 0)
				y = 0;
			if ((i & 1) == 0) {
				v = (0xff & yuv420sp[uvp++]) - 128;
				u = (0xff & yuv420sp[uvp++]) - 128;
			}

			int y1192 = 1192 * y;
			int r = (y1192 + 1634 * v);
			int g = (y1192 - 833 * v - 400 * u);
			int b = (y1192 + 2066 * u);

			if (r < 0)
				r = 0;
			else if (r > 262143)
				r = 262143;
			if (g < 0)
				g = 0;
			else if (g > 262143)
				g = 262143;
			if (b < 0)
				b = 0;
			else if (b > 262143)
				b = 262143;

			rgb[yp] = 0xff000000 | ((r << 6) & 0xff0000)
				| ((g >> 2) & 0xff00) | ((b >> 10) & 0xff);
		}
	}
}