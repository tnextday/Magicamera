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

    bool    m_bVisible;

protected:
    virtual void resizeCoord();
    virtual bool onInit();
    virtual void onDraw( Texture *texutre );

public:
    CoverEngine(void);
    virtual ~CoverEngine(void);

    void setCover(const char* coverPath);

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

    
    

};
