#pragma once
#include "glutils/texture.h"
#include "glutils/baseshader.h"
#include "glutils/framebufferobject.h"
#include "utils/fileutils.h"

class ImageAdjust
{
    BaseShader      m_shader;
    GLfloat         m_QuadData[16];
    FramebufferObject* m_fbo;
    GLuint          m_width;
    GLuint          m_height;

    bool            m_bXfliped;
    bool            m_bYfliped;
    int             m_rorateCount;

    bool            m_bNeedAdjust;

protected:
    

public:
    ImageAdjust(void);
    virtual ~ImageAdjust(void);
    bool init();

    bool isNeedAdjust(){return m_bNeedAdjust;};
    bool apply( Texture* src, Texture* dst );
    GLfloat* getQuadData(){return m_QuadData;};
    void setSize(int w, int h);

    int getWidth(){return m_width;};
    int getHeight() { return m_height;};

    void reset();
    void flip(bool x, bool y);
    void rotate90 (bool clockwise);
};
