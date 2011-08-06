#pragma once
#include "magicengine.h"
#include "sprite.h"

class CoverEngine :
    public MagicEngine
{
private:
    Sprite  *m_img;
    Sprite  *m_conver;

public:
    CoverEngine(void);
    ~CoverEngine(void);

    virtual void update( GLfloat delta );
    virtual void onDraw( Texture *texutre );
    
    virtual void finish();

    virtual bool onInit();

    virtual void start();

    virtual bool isFinished();
    
    virtual bool onTouchDown( float x, float y );
    virtual bool onTouchDrag( float x, float y );
    virtual bool onTouchUp( float x, float y );

    virtual EngineType type();
};
