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

    GLuint      m_width;
    GLuint      m_height;


    Texture     m_OutTex;
    Texture*    m_InTex;

    FramebufferObject* m_fbo;
    
protected:
    virtual void onProcess(Texture* src, Texture* dst) = 0;
    virtual void onResize() = 0;
    virtual void onInit() = 0;

public:
    ImageEffect(void);
    virtual ~ImageEffect(void);

    virtual void Init();

    virtual void process();
    virtual void process(Texture* src, Texture* dst);
    virtual void setInputTex(Texture * tex);
    Texture* getOutTexture(){return &m_OutTex;};
    bool loadFromRes( const char* fileName );
    bool loadFromMemory( const char* buf, int size );
};
