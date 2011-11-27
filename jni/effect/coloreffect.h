#pragma once
#include "effect.h"

#define ColorEffect_Effect_Name "ColorEffect"

//»³¾ÉÉ«µ÷
class ColorEffect : public Effect{

    BaseShader          mShader;
    FramebufferObject   mFBO;
    Texture             mColorIndex;
    GLint               mSrcTexLoc;
    GLint               mLookupTexLoc;
public:
    ColorEffect(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();

};