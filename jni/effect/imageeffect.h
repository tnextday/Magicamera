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

    GLuint      m_width;
    GLuint      m_height;

    Texture     m_OutTex;
    Texture*    m_InTex;

    FramebufferObject* m_fbo;
    
protected:
    virtual void onDoEffect() = 0;
    virtual void onResize() = 0;
    virtual void onInit() = 0;
    bool loadFromFile(const char* fileName);
    bool loadFromMemory(const char* buf, int size);
public:
    ImageEffect(void);
    virtual ~ImageEffect(void);

    virtual void Init();

    virtual void doEffect();
    virtual void setInputTex(Texture * tex);
    Texture* getOutTexture(){return &m_OutTex;};
};
