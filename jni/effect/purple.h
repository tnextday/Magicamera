#pragma once
#include "effect.h"

#define Purple_Effect_Name "Purple"

//×ÏÉ«µ÷
class Purple : public Effect{

    BaseShader          mShader;
    FramebufferObject   mFBO;
    Texture             mColorCurve;
    GLint               mSrcTexLoc;
    GLint               mCurveTexLoc;
public:
    Purple(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();

};