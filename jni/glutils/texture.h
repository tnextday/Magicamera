#ifndef _Texture_h_
#define _Texture_h_

#include <GLES2/gl2.h>
#include "gdx2d/gdx2d.h"



class Texture
{
private:
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
    Texture(const unsigned char *buffer, uint32_t len);
    Texture(const char* texFilePath);
    ~Texture();

    void bind(){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TexHandle);
    };
    void bind(int unit);
    void setWrap(GLfloat wrap_s = GL_REPEAT, GLfloat wrap_t = GL_REPEAT);
    void setFilter(GLfloat min = GL_LINEAR, GLfloat mag = GL_LINEAR);
    void uploadImageData(GLubyte* data, int width, int height, GLenum imageFormat);
    void uploadImageData(GLubyte* data);
    //update压缩过的数据，jpeg,png,tga。。。
    void loadFromMemory(const unsigned char *buffer, uint32_t len);
    bool loadFromFile(const char *filePath);
    void setSize(int w, int h);;
    GLuint getWidth(){return m_Width;};
    GLuint getHeight(){return m_Height;};
    GLuint getTexHandle(){return m_TexHandle;};
    //GL Format
    void setImageFormat(GLenum imageFormat){ m_imageFormat = imageFormat;};
    void init();

private:
    GLuint createGLHandle();
    void deleteGLHandle(GLuint id);
};



#endif // _Texture_h_