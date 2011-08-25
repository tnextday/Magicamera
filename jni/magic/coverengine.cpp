#include "coverengine.h"
#include "actions/moveto.h"
#include "actions/parallel.h"
#include "actions/rotateto2d.h"
#include "utils/easing.h"
#include <time.h>

CoverEngine::CoverEngine(void)
{
    m_img = NULL;
    m_cover = NULL;
    m_nextCover = NULL;
    m_toFinish = false;
    m_bVisible = false;
    m_finished = false;
}

CoverEngine::~CoverEngine(void)
{
    SafeDelete(m_img);
    SafeDelete(m_cover);
    SafeDelete(m_nextCover);
}

void CoverEngine::update( GLfloat delta )
{
    if (!m_cover) return;
    m_cover->update(delta);
    if (m_cover->isActionDone()){
        if (!m_bVisible){
            if (m_toFinish){
                m_finished = true;
            }else if (m_nextCover){
                SafeDelete(m_cover);
                m_cover = m_nextCover;
                m_nextCover = NULL;
                showCover();
            }
        }    
    }
}

bool CoverEngine::onTouchDown( float x, float y )
{
    return false;
}

void CoverEngine::finish()
{
    m_toFinish = true;
    if (!m_bVisible)
        m_finished = true;
    else
        hideCover();
}

bool CoverEngine::onInit()
{
    m_img = new Sprite(m_InTex);
//    m_conver = new Sprite();
    return true;
}

void CoverEngine::start()
{
    m_toFinish = false;
    m_finished = false;
    setCover("./assets/frame/01.png");
}

bool CoverEngine::isFinished()
{
    return m_finished;
}

bool CoverEngine::onTouchDrag( float x, float y )
{
    return false;
}

void CoverEngine::onDraw( Texture *texutre )
{
    m_img->draw(m_shader, texutre);
    if(m_cover){
        m_cover->draw(m_shader);
    }
}

bool CoverEngine::onTouchUp( float x, float y )
{
    return false;
}

EngineType CoverEngine::type()
{
    return EngineType_Cover;
}

void CoverEngine::showCover()
{
    m_cover->setPostion(m_width/2, m_cover->getRegionHeight()/2+m_height);
    m_cover->setRotation(0);
    MoveTo *mt = new MoveTo(m_width/2, m_height/2, 1.5);
    mt->setEasing(new CEaseOutBounce(0.8));
    m_cover->doAction(mt);
    m_bVisible = true;

}

void CoverEngine::hideCover()
{
    Parallel *parallel = new Parallel();
    srand(time(0));
    RotateTo2D * rotate = new RotateTo2D(-10 + rand()%20 , 0.6);
    rotate->setEasing(new CEaseInOutCirc);
    *parallel<<rotate;
    MoveTo *mt = new MoveTo(m_width/2, -m_height/2, 0.8);
    mt->setEasing(new CEaseInQuart);
    *parallel<<mt;
    m_cover->doAction(parallel);
    m_bVisible = false;
}
void CoverEngine::setCover( const char* coverPath )
{
    if (!coverPath){
        return;
    }
    if (m_cover && m_bVisible){
        m_nextCover = new Sprite(coverPath);
        hideCover();
    }else{
        SafeDelete(m_cover);
        m_cover = new Sprite(coverPath);
        showCover();
    }
}