#ifndef _glyuvtexture_h_
#define _glyuvtexture_h_

#include <GLES2/gl2.h>
#include "glutils.h"
#include "utils/mathelpers.h"
#include "framebufferobject.h"
#include "baseshader.h"
#include "magic/imageadjust.h"
#include "glutils/texture.h"

const int YTexId_idx = 0;
const int UVTexId_idx = 1;

class glYUVTexture
{
    BaseShader  m_shader;
    GLint       m_uYtexLoc;
    GLint       m_uUVTexLoc;
    Texture     *m_DstTex;
    GLuint      m_iWidth;
    GLuint      m_iHeight;

    GLuint      m_oWidth;
    GLuint      m_oHeight;

    GLuint      m_YUVTexs[2];

    GLfloat     *m_QuadData;

    ImageAdjust *m_Adjust;

    FramebufferObject* m_fbo;

public:
    glYUVTexture(int w, int h, Texture *tex);
    ~glYUVTexture(void);

    bool init(int w, int h, Texture *tex);

    void setDstTex(Texture *tex);
    void uploadYUVTexImage(char* yuv420sp, int w, int h);

    void setImageAdjust(ImageAdjust *adjust);

private:
    void setDefaultTexParameter(GLuint texId);

};
#endif // _glyuvtexture_h_
