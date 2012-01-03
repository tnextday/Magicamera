#pragma once

#include "glutils/texture.h"
#include "glutils/baseshader.h"
#include "glutils/framebufferobject.h"

//由于effect全部都是渲染到fbo上面，是上下翻转的，这里直接翻转坐标系
const GLfloat G_QuadData[] = {
    // X, Y, U, V
    -1.0f, -1.0f, 0.0f, 1.0f,
    +1.0f, -1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, 1.0f, 0.0f,
    -1.0f, +1.0f, 0.0f, 0.0f,
};


class Effect
{
public:
/*    Effect(void){};*/
    virtual ~Effect(void){};
    virtual void apply(Texture* input, Texture* output) = 0;
    virtual const char* getName() = 0;
    virtual void setParameter(const char* parameterKey, float value) = 0;
    virtual float getParameterValue(const char* parameterKey) = 0;
    virtual const char* getParameterKeys() = 0;
};


