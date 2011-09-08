#pragma once
#include "imageeffect.h"

class BlurEffect :
    public ImageEffect
{
    float   m_stepX;
    float   m_stepY;
    float   m_stepSize;
    GLint   m_stepXLoc;
    GLint   m_stepYLoc;
public:
    BlurEffect(void);
    virtual ~BlurEffect(void);

    virtual void onInit();

    virtual void onResize();

    virtual void onDoEffect();;

    void setStepSize(float ss);
};
