#pragma once
#include "magicengine.h"
#include "glutils/sprite.h"
#include "effect/effect.h"

class EffectEngine :
    public MagicEngine
{
private:
    Sprite      m_img;
    Texture     *m_coverTex;
    Sprite      *m_frame;
    Sprite      *m_nextFrame;
    char        m_framePath[260];
    Effect      *m_effect;
    Texture     *m_effectTex;

    bool    m_toFinish;
    bool    m_finished;

    bool    m_bVisible;

    GLenum      m_sCoverFactor;
    GLenum      m_dCoverFactor;
    GLenum      m_sFrameFactor;
    GLenum      m_dFrameFactor;

protected:
    virtual void resizeCoord(int w, int h);
    virtual bool onInit();
    virtual void draw( Texture *inTex );
    bool loadFrame(const char* framePath);

public:
    EffectEngine(void);
    virtual ~EffectEngine(void);

    void showCover();
    void hideCover();

    virtual void update( GLfloat delta );
    
    virtual void finish();

    virtual void start();

    virtual bool isFinished();
    
    virtual bool onTouchDown( float x, float y );
    virtual bool onTouchDrag( float x, float y );
    virtual bool onTouchUp( float x, float y );

    virtual EngineType type();
    virtual void setInputTexture( Texture* tex);
    virtual void updateInput( Texture* tex);

public:
    void setCover(const char* coverPath);
    void setFrame(const char* framePath);
    //void setFrame( const unsigned char* buffer, uint32_t size );
    void setEffect(const char* effectName);
    const char* getEffectName();
    void setParameter(const char* parameterKey, float value);
    float getParameterValue(const char* parameterKey);
    const char* getParameterKeys();

    virtual void setSize( int w, int h, bool bPreview = true);
};
