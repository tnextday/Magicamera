#pragma once
#include "effect.h"

#define Nostalgia_Effect_Name "Nostalgia"

//»³¾ÉÉ«µ÷
class Nostalgia : public Effect{

    BaseShader  mShader;
    FramebufferObject   mFBO;

public:
    Nostalgia(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};