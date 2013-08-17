#pragma once
#include "effect.h"

#define InfraRed_Effect_Name "InfraRed"

//反转片
class InfraRed : public Effect{

    BaseShader  mShader;
    FramebufferObject   mFBO;

public:
    InfraRed(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};