#pragma once
#include "magicengine.h"
#include "glutils/sprite.h"
#include "effect/imageeffect.h"

class CoverEngine :
    public MagicEngine
{
private:
    Sprite      m_img;
    Sprite      *m_cover;
    Sprite      *m_nextCover;
    ImageEffect *m_effect;
    Texture     *m_effectTex;

    bool    m_toFinish;
    bool    m_finished;

    bool    m_bVisible;
    

protected:
    virtual void resizeCoord();
    virtual bool onInit();
    virtual void onDraw( Texture *texutre );
    void doEffect(Texture* tex);

public:
    CoverEngine(void);
    virtual ~CoverEngine(void);


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
    void setCover( const unsigned char* buffer, uint32_t size );
    void setEffect(const char* effectPath);
};
