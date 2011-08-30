#pragma once
#include "imageeffect.h"

class BlurEffect :
    public ImageEffect
{
public:
    BlurEffect(void);
    virtual ~BlurEffect(void);

    virtual void onInit();

    virtual void onResize();

    virtual void onDoEffect(){};
};
