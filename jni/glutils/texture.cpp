#include <stdio.h>
#include "texture.h"
#include "glutils.h"


Texture::Texture()
{
    m_TexHandle = 0;
}

int getGLInternalFormat(int format) {
    switch(format) {
        case GDX2D_FORMAT_ALPHA:
            return GL_ALPHA;
        case GDX2D_FORMAT_LUMINANCE_ALPHA:
            return GL_LUMINANCE_ALPHA;
        case GDX2D_FORMAT_RGB888:
        case GDX2D_FORMAT_RGB565:
            return GL_RGB;
        case GDX2D_FORMAT_RGBA8888:
        case GDX2D_FORMAT_RGBA4444:
            return GL_RGBA;
        default: 
            return GL_RGB;
            //throw new GdxRuntimeException("unknown format: " + format);
    }
}
int getGLType(int format) {
    switch(format) {
        case GDX2D_FORMAT_ALPHA:            
        case GDX2D_FORMAT_LUMINANCE_ALPHA:
        case GDX2D_FORMAT_RGB888:
        case GDX2D_FORMAT_RGBA8888:
            return GL_UNSIGNED_BYTE;
        case GDX2D_FORMAT_RGB565:
            return GL_UNSIGNED_SHORT_5_6_5;
        case GDX2D_FORMAT_RGBA4444:
            return GL_UNSIGNED_SHORT_4_4_4_4;
        default:
            return GL_UNSIGNED_BYTE;
    }
}


Texture::Texture( const unsigned char *buffer, uint32_t len)
{
    init();
    loadFromMemory(buffer, len);
}

Texture::Texture(const char* texFilePath )
{
    init();
    loadFromFile(texFilePath);
}

Texture::~Texture()
{
    deleteGLHandle(m_TexHandle);
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
    int glformat = getGLInternalFormat(m_imageFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, getGLType(m_imageFormat), data);
    checkGlError("glTexImage2D");
}

void Texture::uploadImageData( GLubyte* data )
{
    uploadImageData(data, m_Width, m_Height, m_imageFormat);
}

void Texture::loadFromMemory( const unsigned char *buffer, uint32_t len)
{
    gdx2d_pixmap* pixmap = gdx2d_load(buffer, len);
    m_imageFormat = pixmap->format;
    m_Width = pixmap->width;
    m_Height = pixmap->height;
    //TODO 貌似需要字节对齐 
    LOGI("Decoder Image : %d,%d format:%d\n", pixmap->width, pixmap->height, pixmap->format);
    uploadImageData((GLubyte*)(pixmap->pixels), pixmap->width, pixmap->height, m_imageFormat);
    gdx2d_free(pixmap);
}
GLuint Texture::createGLHandle()
{
    GLuint texID[1];
    glGenTextures(1, texID);
    return texID[0];
}

void Texture::init()
{
    m_TexHandle = createGLHandle();
    setWrap();
    setFilter();
}

void Texture::setSize( int w, int h )
{
    if (m_Width == w && m_Height == h) return;
    m_Width = w; 
    m_Height = h;
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

bool Texture::loadFromFile( const char *filePath )
{
    if (!m_TexHandle){
        init();
    }
    bool result = true;
    FILE *fp = fopen(filePath, "rb");
    if (!fp) {
        LOGE("Can't open file: %s\n", filePath);
        return false;
    }
    char* buffer = NULL;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);            // determine file size so we can fill it in later if FileSize == 0
    if (size <= 0) {
        fclose(fp);
        return false;
    }
    fseek(fp, 0, SEEK_SET);
    buffer = new char[size];

    if (fread(buffer, sizeof(char), size, fp) > 0){
        loadFromMemory((unsigned char*)buffer, size);
    }else{
        LOGE("Can't load texture: %s\n", filePath);
        result = false;
    }
    fclose(fp);
    if (buffer){
        delete[] buffer;
        buffer = NULL;
    }
    return result;
}

void Texture::deleteGLHandle( GLuint id )
{
    if (id){
        GLuint texID[1];
        texID[0] = id;
        glDeleteTextures(1, texID);
    }
}