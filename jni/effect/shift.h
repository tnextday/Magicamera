#pragma once
#include "effect.h"


#define Shift_Effect_Name "Shift"

class Shift :
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
    GLint               mEdge00Loc;
    GLint               mEdge01Loc;
    GLint               mEdge10Loc;
    GLint               mEdge11Loc;
    float               mBlurStep;
    float               mEdge0;
    float               mEdge1;
public:
    Shift(void);
    virtual ~Shift(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
};
