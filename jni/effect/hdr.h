#pragma once
#include "effect.h"


#define HDR_Effect_Name "HDR"

class HDR :
    public Effect
{
private:
    BaseShader          mHBlur;
    BaseShader          mVBlur;
    BaseShader          mFinal;
    Texture             mTmpTex;
    Texture             mTmpTex2;
    FramebufferObject   mFBO;
    GLint               mHBlurSizeLoc;
    GLint               mVBlurSizeLoc;
    GLint               mExposureLoc;
    GLint               mSrcTexLoc;
    GLint               mLightTexLoc;
    float               mBlurStep;
    float               mExposure;
public:
    HDR(void);
    virtual ~HDR(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};
