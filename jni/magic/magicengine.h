#pragma once
#include <GLES2/gl2.h>
#include "glutils/texture.h"
#include "glutils/baseshader.h"
#include "ui/touchevent.h"
#include "glutils/framebufferobject.h"
#include "glutils/sprite.h"
#include "utils/fileutils.h"

enum EngineType {
    EngineType_None,
    EngineType_Mesh,
    EngineType_Cover,
    ENgineType_Effect,
    EngineType_Kaleidoscope
};

class EngineOutChange{
public:
    virtual void onEngineOutChange(Texture *tex) = 0;
};

class MagicEngine : public TouchEvent{
protected:
    BaseShader      m_shader;

    Texture*        m_InTex;
    Texture         m_OutTex;

    GLfloat         m_width;
    GLfloat         m_height;
    GLfloat         m_aspectRatio;

    GLenum          m_sfactor;
    GLenum          m_dfactor;

    FramebufferObject*      m_fbo;
    FileUtils*      m_ioCallBack;

    EngineOutChange* m_onOutputResize;

protected:
    void draw(Texture *texutre = NULL);
    virtual bool onInit() = 0;
    virtual void onDraw(Texture *texutre) = 0;
    virtual void resizeCoord();
public:
    MagicEngine();
    MagicEngine(Texture* SrcTex);
    //声明为虚函数，确保子类可以被释放
    virtual	~MagicEngine();

    bool initEngine(Texture* SrcTex) ;
    void setSize(int w, int h);
    virtual void setInputTexture(Texture* val);
    virtual void updateInput(Texture* val){};
    Texture* getOutTexture() {return &m_OutTex;};

    void drawImage();

    void tackPicture(Texture *texutre = NULL);
    void tackPicture(const char* data, long len);
    void tackPicture(const char* imagePath);
    void tackPicture(const char* data, int w, int h, int format);

    void SetIOCallBack(FileUtils* val);
    void setOutputResize(EngineOutChange* val) { m_onOutputResize = val; }

    GLfloat getWidth(){return m_width;}
    GLfloat getHeigth(){return m_height;} 

    void setBlendFunc(GLenum sfactor, GLenum dfactor);

    virtual void update(GLfloat delta) = 0;
    virtual void start() = 0;
    virtual void finish() = 0;
    virtual bool isFinished() = 0;
    virtual EngineType type() = 0;

    virtual bool onTouchDown( float x, float y );
    virtual bool onTouchDrag( float x, float y );
    virtual bool onTouchUp( float x, float y );
};
