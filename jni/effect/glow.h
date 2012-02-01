#pragma once
#include "effect.h"


#define Glow_Effect_Name "Glow"

class Glow :
    public Effect
{
private:
    BaseShader          mHBlur;
    BaseShader          mFinal;
    Texture             mTmpTex;
    FramebufferObject   mFBO;
    GLint               mHBlurSizeLoc;
    GLint               mVBlurSizeLoc;
    GLint               mExposureLoc;
    GLint               mSrcTexLoc;
    GLint               mRTSenceLoc;
    float               mBlurStep;
public:
    Glow(void);
    virtual ~Glow(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};
