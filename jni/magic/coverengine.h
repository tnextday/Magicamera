#pragma once
#include "magicengine.h"
#include "glutils/sprite.h"

class CoverEngine :
    public MagicEngine
{
private:
    Sprite  *m_img;
    Sprite  *m_cover;
    Sprite  *m_nextCover;

    bool    m_toFinish;

    bool    m_finished;

public:
    CoverEngine(void);
    virtual ~CoverEngine(void);

    void setCover(const char* coverPath);

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
