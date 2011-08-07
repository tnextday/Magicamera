
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
    // Method:    FramebufferObject ���캯��
    // Parameter: bool bColorBUffer �Ƿ񴴽�color buffer
    // Parameter: bool bDepthBuffer �Ƿ񴴽�depth buffer
    // ColorBufferֻ֧��16λɫ�������֧�ָ�����ɫ��Ҫʹ��texture2d����
    //************************************
    FramebufferObject(bool bColorBUffer = false, bool bDepthBuffer = false);
    ~FramebufferObject(void);

    void bind();
    void unbind();
    //�˺�����ɫ����egl����ɫ����ͬ
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