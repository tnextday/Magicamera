#include "coverengine.h"
#include "actions/moveto.h"
#include "actions/parallel.h"
#include "actions/rotateto2d.h"
#include "utils/easing.h"
#include "utils/mathelpers.h"
#include <time.h>
#include <stdlib.h>

CoverEngine::CoverEngine(void)
{
    m_cover = NULL;
    m_nextCover = NULL;
    m_effect = NULL;
    m_effectTex = NULL;
    m_toFinish = false;
    m_bVisible = false;
    m_finished = false;
}

CoverEngine::~CoverEngine(void)
{
    SafeDelete(m_cover);
    SafeDelete(m_nextCover);
    SafeDelete(m_effect);
    SafeDelete(m_effectTex);
}

void CoverEngine::update( GLfloat delta )
{
    m_img.update(delta);
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
//    m_img.setTexture(m_InTex);
//    m_conver = new Sprite();
    return true;
}

void CoverEngine::start()
{
    m_toFinish = false;
    m_finished = false;
    //uint32_t size;
    //unsigned char* date = m_ioCallBack->readRes("frames/02.png", size);
    //setCover(date, size);
    //delete [] date;
    setEffect("shaders/Blur.sp");
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
    if (texutre)
        doEffect(texutre);
    Texture* srcTex = texutre;
    if (m_effectTex)
        srcTex = m_effectTex;
    m_img.draw(&m_shader, srcTex);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(m_cover){
        m_cover->draw(&m_shader);
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
    resizeCoord();
    m_cover->setPostion(0, m_aspectRatio);
    m_cover->setRotation(0);
    MoveTo *mt = new MoveTo(0, 0, 1.5);
    mt->setEasing(new CEaseOutBounce(0.6));
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
    MoveTo *mt = new MoveTo(0, -1.5, 1.0);
    mt->setEasing(new CEaseInQuart);
    *parallel<<mt;
    m_cover->doAction(parallel);
    m_bVisible = false;
}

void CoverEngine::setCover( const unsigned char* buffer, uint32_t size )
{
    if (!buffer || !size){
        return;
    }
    
    if (m_cover && m_bVisible){
        SafeDelete(m_nextCover);
        m_nextCover = new Sprite(buffer, size);
        hideCover();
    }else{
        SafeDelete(m_cover);
        m_cover = new Sprite(buffer, size);
        showCover();
    }
}

void CoverEngine::setCover( const char* coverPath)
{
    if (!coverPath){
        return;
    }
    if (m_cover && m_bVisible){
        SafeDelete(m_nextCover);
        m_nextCover = new Sprite(coverPath);
        hideCover();
    }else{
        SafeDelete(m_cover);
        m_cover = new Sprite(coverPath);
        showCover();
    }
}


void CoverEngine::resizeCoord()
{
    if (m_cover)
    {
        //ÖØÖÃ×ø±êÏµ
        m_width = m_cover->getRegionWidth();
        m_height = m_cover->getRegionHeight();
        m_aspectRatio = m_width/m_height;
        setSize(m_width, m_height);
        m_shader.ortho(-m_aspectRatio/2, m_aspectRatio/2, -0.5, 0.5, -10, 10);
        if (m_onOutputResize){
            m_onOutputResize->onEngineOutChange(&m_OutTex);
        }
    }else{
        MagicEngine::resizeCoord();
    }
}

void CoverEngine::setInputTexture( Texture* tex )
{
    m_img.setTexture(tex);
    MagicEngine::setInputTexture(tex);
    doEffect(tex);
}

void CoverEngine::updateInput( Texture* tex )
{
    doEffect(tex);
}

void CoverEngine::setEffect( const char* effectPath )
{
    if (!effectPath){
        SafeDelete(m_effect);
        SafeDelete(m_effectTex);
    }
    if (!m_effectTex){
        m_effectTex = new Texture;
        m_effectTex->init();
    }
    if (!m_effect){
        m_effect = new ImageEffect();
    }
    m_effect->loadFromRes(effectPath);
    doEffect(m_InTex);
}

void CoverEngine::doEffect( Texture* tex )
{
    if (!m_effect) return;
    m_effect->process(tex, m_effectTex);
}