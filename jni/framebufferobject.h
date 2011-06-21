
#ifndef _framebufferobject_h_
#define _framebufferobject_h_

#include <GLES2\gl2.h>

class FramebufferObject
{

private:
    GLuint m_fboId[1];
    GLuint m_colorBuffer;
    GLuint m_depthBuffer;
    GLuint m_width;
    GLuint m_height;
    bool   m_bUseColorBuffer;
    bool   m_bUseDepthBuffer;

public:
    //************************************
    // Method:    FramebufferObject 构造函数
    // Parameter: bool bColorBUffer 是否创建color buffer
    // Parameter: bool bDepthBuffer 是否创建depth buffer
    //************************************
    FramebufferObject(bool bColorBUffer = false, bool bDepthBuffer = false);
    ~FramebufferObject(void);

    void bind();
    void unbind();
    void texture2d(GLuint texId);
    void resizeBuffers(int w, int h);
    bool check_status();

private:
    void createColorBuffer();
    void createDepthBuffer();
    GLuint genRenderbuffer();
    void deleteRenderBuffer(GLuint bufferId);
};
#endif // _framebufferobject_h_
