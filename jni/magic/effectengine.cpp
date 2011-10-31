#include "effectengine.h"
#include "actions/moveto.h"
#include "actions/parallel.h"
#include "actions/rotateto2d.h"
#include "utils/easing.h"
#include "utils/mathelpers.h"
#include <time.h>
#include <stdlib.h>
#include "utils/packageloader.h"
#include "effect/effectfactory.h"

EffectEngine::EffectEngine(void)
{
    m_frame = NULL;
    m_framePath[0] = 0;
    m_effect = NULL;
    m_effectTex = NULL;
    m_coverTex = NULL; 
    m_toFinish = false;
    m_bVisible = false;
    m_finished = false;
    m_sFrameFactor = GL_SRC_ALPHA;
    m_dFrameFactor = GL_ONE_MINUS_SRC_ALPHA;
    m_sCoverFactor = GL_ONE;
    m_dCoverFactor = GL_ONE;
}

EffectEngine::~EffectEngine(void)
{
    SafeDelete(m_frame);
    SafeDelete(m_effect);
    SafeDelete(m_effectTex);
    SafeDelete(m_coverTex);
}

void EffectEngine::update( GLfloat delta )
{
    m_img.update(delta);
    if (!m_frame) return;
    m_frame->update(delta);
    if (m_frame->isActionDone()){
        if (!m_bVisible){
            SafeDelete(m_frame);
            if (m_toFinish){
                m_finished = true;
            }else if (loadFrame(m_framePath)){
                m_framePath[0] = 0;
                showCover();
            }else{
                resizeCoord(m_InTex->getWidth(), m_InTex->getHeight());
            }
        }    
    }
}

bool EffectEngine::onTouchDown( float x, float y )
{
    return false;
}

void EffectEngine::finish()
{
    m_toFinish = true;
    if (!m_bVisible)
        m_finished = true;
    else
        hideCover();
}

bool EffectEngine::onInit()
{
//    m_img.setTexture(m_InTex);
//    m_conver = new Sprite();
    return true;
}

void EffectEngine::start()
{
    m_toFinish = false;
    m_finished = false;
}

bool EffectEngine::isFinished()
{
    return m_finished;
}

bool EffectEngine::onTouchDrag( float x, float y )
{
    return false;
}

void EffectEngine::onDraw( Texture *texutre )
{
    if (texutre)
        doEffect(texutre);
    else
        doEffect(m_InTex);
    m_fbo->bind();
    Texture* srcTex = texutre;
    if (m_effectTex)
        srcTex = m_effectTex;
    glDisable(GL_BLEND);
    m_img.draw(&m_shader, srcTex);
    glEnable(GL_BLEND);
    if (m_coverTex){
        glBlendFunc(m_sCoverFactor, m_dCoverFactor);
        m_img.draw(&m_shader, m_coverTex);
    }
    if(m_frame){
        glBlendFunc(m_sFrameFactor, m_dFrameFactor);
        m_frame->draw(&m_shader);
    }
}

bool EffectEngine::onTouchUp( float x, float y )
{
    return false;
}

EngineType EffectEngine::type()
{
    return EngineType_Effect;
}

void EffectEngine::showCover()
{
    resizeCoord(0, 0);
    m_frame->setPostion(0, m_aspectRatio);
    m_frame->setRotation(0);
    MoveTo *mt = new MoveTo(0, 0, 1.5);
    mt->setEasing(new CEaseOutBounce(0.6));
    m_frame->doAction(mt);
    m_bVisible = true;
}

void EffectEngine::hideCover()
{
    Parallel *parallel = new Parallel();
    srand(time(0));
    RotateTo2D * rotate = new RotateTo2D(-10 + rand()%20 , 0.6);
    rotate->setEasing(new CEaseInOutCirc);
    *parallel<<rotate;
    MoveTo *mt = new MoveTo(0, -1.5, 1.0);
    mt->setEasing(new CEaseInQuart);
    *parallel<<mt;
    m_frame->doAction(parallel);
    m_bVisible = false;
}

// void EffectEngine::setFrame( const unsigned char* buffer, uint32_t size )
// {
//     if (!buffer || !size){
//         SafeDelete(m_nextFrame);
//         hideCover();
//         return;
//     }
//     
//     if (m_frame && m_bVisible){
//         SafeDelete(m_nextFrame);
//         m_nextFrame = new Sprite(buffer, size);
//         hideCover();
//     }else{
//         SafeDelete(m_frame);
//         m_frame = new Sprite(buffer, size);
//         showCover();
//     }
// }

void EffectEngine::setFrame( const char* framePath)
{
    m_framePath[0] = 0;
    if (!framePath){
        hideCover();
        return;
    }
    if (m_frame && m_bVisible){
        strncpy(m_framePath, framePath, 260);
        hideCover();
    }else{
        if (loadFrame(framePath))
            showCover();
    }
}


void EffectEngine::setCover( const char* coverPath )
{
    SafeDelete(m_coverTex);
    if (coverPath){
        BlendTexLoader loader;
        if (loader.loadFromFile(coverPath)){
            m_coverTex = new Texture(loader.getData(), loader.getDataSize());
            m_sCoverFactor = loader.getSrcFactor();
            m_dCoverFactor = loader.getDstFactor();
        }
    }
}

void EffectEngine::setInputTexture( Texture* tex )
{
    m_img.setTexture(tex);
    MagicEngine::setInputTexture(tex);
    doEffect(tex);
}

void EffectEngine::updateInput( Texture* tex )
{
    doEffect(tex);
}

void EffectEngine::setEffect( const char* effectName )
{
    SafeDelete(m_effect);
    
    m_effect = createEffect(effectName);
    if (!m_effect){
        SafeDelete(m_effectTex);
    }else if(!m_effectTex){
        m_effectTex = new Texture;
        m_effectTex->init();
    }
    doEffect(m_InTex);
}

const char* EffectEngine::getEffectName(){
    if (m_effect)
        return m_effect->getName();
    return NULL;
}

void EffectEngine::setParameter(const char* parameterKey, float value){
    if (m_effect){
        m_effect->setParameter(parameterKey, value);
        doEffect(m_InTex);
    }
}

float EffectEngine::getParameterValue(const char* parameterKey){
    if (m_effect)
        return m_effect->getParameterValue(parameterKey);
    return 0;
}

const char* EffectEngine::getParameterKeys(){
    if (m_effect)
        return m_effect->getParameterKeys();
    return NULL;
}

void EffectEngine::doEffect( Texture* tex )
{
    if (!m_effect) return;
    m_effectTex->setSize(m_width, m_height);
    m_effect->apply(tex, m_effectTex);
}

void EffectEngine::resizeCoord(int w, int h)
{
    if (m_frame)
    {
        w = m_frame->getRegionWidth();
        h = m_frame->getRegionHeight();
    }
    MagicEngine::resizeCoord(w, h);
}

void EffectEngine::setSize( int w, int h , bool bPreview /*= true*/)
{
    if (m_frame){
        w = m_frame->getRegionWidth();
        h = m_frame->getRegionHeight();
    }
    MagicEngine::setSize(w, h, bPreview);
}

bool EffectEngine::loadFrame( const char* framePath )
{
    SafeDelete(m_frame);
    if (framePath){
        BlendTexLoader loader;
        if (loader.loadFromFile(framePath)){
            m_frame = new Sprite(loader.getData(), loader.getDataSize());
            m_sFrameFactor = loader.getSrcFactor();
            m_dFrameFactor = loader.getDstFactor();
            return true;
        }
    }
    return false;
}