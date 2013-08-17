#pragma once
#include "effect.h"

#define ScanLine_Effect_Name "ScanLine"

class ScanLine : public Effect{

    BaseShader  mShader;
    FramebufferObject   mFBO;

public:
    ScanLine(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};