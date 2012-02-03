#pragma once
#include "effect.h"

#define LEMO_Effect_Name "LEMO"

//怀旧色调
class LEMO : public Effect{

    BaseShader          mShader;
    FramebufferObject   mFBO;
    Texture             mColorCurve;
    GLint               mSrcTexLoc;
    GLint               mCurveTexLoc;
public:
    LEMO(void);
    virtual void apply( Texture* input, Texture* output );
    virtual const char* getName();
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();

};