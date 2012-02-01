#pragma once
#include "effect.h"

//��ɫ����
class CurveEffect : public Effect{

    BaseShader          mShader;
    FramebufferObject   mFBO;
    Texture             mColorCurve;
    GLint               mSrcTexLoc;
    GLint               mCurveTexLoc;
public:
    CurveEffect(void);
    virtual const char* getName() = 0;
    virtual void apply( Texture* input, Texture* output );
    virtual void setParameter( const char* parameterKey, float value );
    virtual float getParameterValue( const char* parameterKey );
    virtual const char* getParameterKeys();
    void setCurveTex(const char* texPath);

};