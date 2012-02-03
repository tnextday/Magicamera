
#ifndef _framebufferobject_h_
#define _framebufferobject_h_

#ifdef __MAC_NA
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#endif

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
    // Method:    FramebufferObject ���캯��
    // Parameter: bool bColorBUffer �Ƿ񴴽�color buffer
    // Parameter: bool bDepthBuffer �Ƿ񴴽�depth buffer
    // ColorBufferֻ֧��16λɫ�������֧�ָ����ɫ��Ҫʹ��texture2d����
    //************************************
    FramebufferObject(bool bDepthBuffer = false);
    ~FramebufferObject(void);

    void bind(){ glBindFramebuffer(GL_FRAMEBUFFER, m_fboId[0]); };
    void bindWithTexture(GLuint texId);
    void unbind(){ glBindFramebuffer(GL_FRAMEBUFFER, 0); };
    //�˺����ɫ����egl����ɫ����ͬ
    void texture2d(GLuint texId);
    void resizeBuffers(int w, int h);
    bool check_status();

private:
    void createDepthBuffer();
    GLuint genRenderbuffer();
    void deleteRenderBuffer(GLuint bufferId);
};
#endif // _framebufferobject_h_
