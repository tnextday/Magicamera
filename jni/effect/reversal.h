#pragma once
#include "effect.h"

#define Reversal_Effect_Name "Reversal"

//·´×ªÆ¬
class Reversal : public Effect{

    BaseShader  mShader;
    FramebufferObject   mFBO;

public:
    Reversal(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};