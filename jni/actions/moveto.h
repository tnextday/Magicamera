#pragma once
#include "actions/action.h"

class MoveTo :public Action
{
    float m_dstX;
    float m_dstY;
    float m_deltaX;
    float m_deltaY;
    float m_startX;
    float m_startY;
public:
    MoveTo(float x, float y, float duration);

    virtual void update( float time );

    virtual void setTarget( Sprite * val );
};
