
#ifndef _framebufferobject_h_
#define _framebufferobject_h_

#include <GLES2\gl2.h>

class FramebufferObject
{

private:
    GLuint m_fboId[1];
    GLuint m_colorBuffer;
    GLuint m_depthBuffer;

public:
    FramebufferObject(void);
    ~FramebufferObject(void);

    void bind();
    void unbind();
    void texture2d(GLuint texId);

    void resizeColorBuffer(int w, int h);
    void resizeDepthBuffer(int w, int h);

    bool check_status();

    GLuint genRenderbuffer();
    void deleteRenderBuffer(GLuint bufferId);

    
};
#endif // _framebufferobject_h_
