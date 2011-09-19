#pragma once
#include "glutils\baseshader.h"
#include "glutils\textureregion.h"
#include "glutils\framebufferobject.h"
#include "utils\fileutils.h"

class ImageEffect
{
protected:
    BaseShader  m_shader;
    GLint       m_imgTexLoc;

    GLint       m_effectTexLoc;
    GLint       m_texWidthLoc;
    GLint       m_texHeightLoc;

    FramebufferObject* m_fbo;
    

public:
    ImageEffect(void);
    ImageEffect(const char* fileName);
    virtual ~ImageEffect(void);
    virtual void process(Texture* src, Texture* dst);
    bool loadFromRes( const char* fileName );
    bool loadFromMemory( const char* buf, int size );
};
