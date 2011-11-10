#pragma once
#include "effect.h"

#define Overlay_Effect_Name "Overlay"

class Overlay : public Effect{

    BaseShader  mShader;
    FramebufferObject   mFBO;
    GLenum      m_sCoverFactor;
    GLenum      m_dCoverFactor;

public:
    Overlay(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();

    void setBlendFunc(GLenum sfactor, GLenum dfactor);
};