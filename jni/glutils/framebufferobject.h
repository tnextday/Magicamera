
#ifndef _framebufferobject_h_
#define _framebufferobject_h_

#include <GLES2\gl2.h>

class FramebufferObject
{

private:
    GLuint m_fboId[1];
    GLuint m_depthBuffer;
    GLuint m_width;
    GLuint m_height;
    bool   m_bUseDepthBuffer;

public:
    //************************************
    // Method:    FramebufferObject 构造函数
    // Parameter: bool bColorBUffer 是否创建color buffer
    // Parameter: bool bDepthBuffer 是否创建depth buffer
    // ColorBuffer只支持16位色，如果想支持更高颜色，要使用texture2d纹理
    //************************************
    FramebufferObject(bool bDepthBuffer = false);
    ~FramebufferObject(void);

    void bind(){ glBindFramebuffer(GL_FRAMEBUFFER, m_fboId[0]); };
    void bindWithTexture(GLuint texId);
    void unbind(){ glBindFramebuffer(GL_FRAMEBUFFER, 0); };
    //此函数的色深与egl环境色深相同
    void texture2d(GLuint texId);
    void resizeBuffers(int w, int h);
    bool check_status();

private:
    void createDepthBuffer();
    GLuint genRenderbuffer();
    void deleteRenderBuffer(GLuint bufferId);
};
#endif // _framebufferobject_h_
