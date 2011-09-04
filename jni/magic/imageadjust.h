#pragma once
#include "glutils/texture.h"
#include "glutils/baseshader.h"
#include "glutils/framebufferobject.h"
#include "utils/fileutils.h"

class AdjustChange{
public:
    virtual void onAdjustChange(Texture *tex) = 0;
};

class ImageAdjust
{
    BaseShader      m_shader;
    Texture*        m_InTex;
    Texture         m_OutTex;
    GLfloat         m_QuadData[16];
    FramebufferObject* m_fbo;
    GLuint          m_width;
    GLuint          m_height;
    AdjustChange*   m_onOutputChange;

    bool            m_bXfliped;
    bool            m_bYfliped;
    int             m_rorateCount;

    bool            m_bNeedAdjust;

protected:
    void resize();

public:
    ImageAdjust(void);
    virtual ~ImageAdjust(void);
    void setInputTexture(Texture* val);
   
    void setOnOutputChange(AdjustChange* val);
    Texture* getOutTexture();;

    bool init();
    void drawImage();
    void reset();

    void flip(bool x, bool y);
    void rotate90 (bool clockwise);
};
