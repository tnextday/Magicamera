#pragma once
#include "effect.h"


#define Microspur_Effect_Name "Microspur"

class Microspur :
    public Effect
{
private:
    BaseShader          mHBlur;
    BaseShader          mFinal;
    Texture             mTmpTex;
    FramebufferObject   mFBO;
    GLint               mHBlurSizeLoc;
    GLint               mVBlurSizeLoc;
    GLint               mSrcTexLoc;
    GLint               mRTSenceLoc;
    GLint               mEdge0Loc;
    GLint               mEdge1Loc;
    GLint               mAspectLoc;
    float               mBlurStep;
    float               mEdge0;
    float               mEdge1;
public:
    Microspur(void);
    virtual ~Microspur(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};
