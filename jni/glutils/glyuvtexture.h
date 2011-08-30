#ifndef _glyuvtexture_h_
#define _glyuvtexture_h_

#include <GLES2/gl2.h>
#include "glutils.h"
#include "utils/mathelpers.h"
#include "framebufferobject.h"
#include "baseshader.h"

const int YTexId_idx = 0;
const int UVTexId_idx = 1;

class glYUVTexture
{
    BaseShader  m_shader;
    GLuint      m_uYtexLoc;
    GLuint      m_uUVTexLoc;

    GLuint      m_width;
    GLuint      m_height;

    GLuint      m_YUVTexs[2];

    FramebufferObject* m_fbo;

public:
    glYUVTexture(int w, int h, GLuint texid);
    ~glYUVTexture(void);

    bool init(int w, int h, GLuint texid);
    void setTargetTexId(GLuint texid);
    void uploadYUVTexImage(char* yuv420sp, int w, int h);

private:
    void setDefaultTexParameter(GLuint texId);

};
#endif // _glyuvtexture_h_
