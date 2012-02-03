#pragma once

#include "glutils/texture.h"
#include "glutils/baseshader.h"
#include "ui/touchevent.h"
#include "glutils/framebufferobject.h"
#include "glutils/sprite.h"
#include "utils/fileutils.h"

enum EngineType {
    EngineType_None = 0,
    EngineType_Mesh,
    EngineType_Effect,
    EngineType_Kaleidoscope,
    EngineType_Size
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
    GLuint         m_width;
    GLuint         m_height;
    GLuint          m_MaxWidth;
    GLuint          m_MaxHeight;
    GLfloat         m_aspectRatio;
    GLuint          m_PreviewWidth;
    GLuint          m_PreviewHeight;

    FramebufferObject*      m_fbo;
    FileUtils*      m_ioCallBack;

    EngineOutChange* m_onOutputResize;

    bool           m_bUpdated;
    bool           m_bReDraw;

protected:
    virtual void draw(Texture *inTex = NULL) = 0;
    virtual bool onInit() = 0;
    virtual void resizeCoord(int w, int h);
public:
    MagicEngine();
/*    MagicEngine(Texture* SrcTex);*/
    //声明为虚函数，确保子类可以被释放
    virtual	~MagicEngine();

    bool initEngine(Texture* SrcTex) ;
    virtual void setSize(int w, int h, bool bPreview = true);
    virtual void setInputTexture(Texture* val);
    virtual void updateInput(Texture* val){m_bReDraw = true;};
    Texture* getOutTexture() {return &m_OutTex;};

    void drawImage();

    void tackPicture(Texture *texutre = NULL);

    void SetIOCallBack(FileUtils* val);
    void setOutputResize(EngineOutChange* val) { m_onOutputResize = val; }

    GLuint getWidth(){return m_width;}
    GLuint getHeigth(){return m_height;} 

    void setPreviewSize(GLuint w, GLuint h);
    void setMaxOutputSize(GLuint w, GLuint h);

    virtual void update(GLfloat delta) = 0;
    virtual void start() = 0;
    virtual void finish() = 0;
    virtual bool isFinished() = 0;
    virtual EngineType type() = 0;

    virtual bool onTouchDown( float x, float y );
    virtual bool onTouchDrag( float x, float y );
    virtual bool onTouchUp( float x, float y );
};
