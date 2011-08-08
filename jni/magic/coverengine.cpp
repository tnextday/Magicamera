#include "coverengine.h"

CoverEngine::CoverEngine(void)
{
    m_img = NULL;
    m_cover = NULL;
    m_nextCover = NULL;
    m_toFinish = false;
}

CoverEngine::~CoverEngine(void)
{
    SafeDelete(m_img);
    SafeDelete(m_cover);
    SafeDelete(m_nextCover);
}

void CoverEngine::update( GLfloat delta )
{

}

bool CoverEngine::onTouchDown( float x, float y )
{
    return false;
}

void CoverEngine::finish()
{
    
}

bool CoverEngine::onInit()
{
    m_img = new Sprite(m_InTex);
//    m_conver = new Sprite();
    return true;
}

void CoverEngine::start()
{

}

bool CoverEngine::isFinished()
{
    return false;
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

void CoverEngine::setCover( const char* coverPath )
{

}