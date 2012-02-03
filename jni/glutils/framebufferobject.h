
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
    // Method:    FramebufferObject 锟斤拷锟届函锟斤拷
    // Parameter: bool bColorBUffer 锟角否创斤拷color buffer
    // Parameter: bool bDepthBuffer 锟角否创斤拷depth buffer
    // ColorBuffer只支锟斤拷16位色锟斤拷锟斤拷锟斤拷锟街э拷指锟斤拷锟斤拷色锟斤拷要使锟斤拷texture2d锟斤拷锟斤拷
    //************************************
    FramebufferObject(bool bDepthBuffer = false);
    ~FramebufferObject(void);

    void bind(){ glBindFramebuffer(GL_FRAMEBUFFER, m_fboId[0]); };
    void bindWithTexture(GLuint texId);
    void unbind(){ glBindFramebuffer(GL_FRAMEBUFFER, 0); };
    //锟剿猴拷锟斤拷锟缴拷锟斤拷锟絜gl锟斤拷锟斤拷色锟斤拷锟斤拷同
    void texture2d(GLuint texId);
    void resizeBuffers(int w, int h);
    bool check_status();

private:
    void createDepthBuffer();
    GLuint genRenderbuffer();
    void deleteRenderBuffer(GLuint bufferId);
};
#endif // _framebufferobject_h_
