#pragma once
#include "actions/action.h"

class RotateTo2D : public Action
{
    float m_dstAngle;
    float m_startAngle;
    float m_deltaAngle;
public:
    RotateTo2D(float ang, float duration);

    virtual void setTarget( Sprite * val );

    virtual void update( float time );
};
