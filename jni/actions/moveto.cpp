#include "moveto.h"

MoveTo::MoveTo(float x, float y, float duration)
        :Action(duration)
{
    m_dstX = x;
    m_dstY = y;
    m_deltaX = 0;
    m_deltaY = 0;
    m_startX = 0;
    m_startY = 0;
}


void MoveTo::update( float time )
{
    m_target->setPostion(m_startX + m_deltaX*time,
                         m_startY + m_deltaY*time);
}

void MoveTo::onStart()
{
    m_startX = m_target->getX();
    m_startY = m_target->getY();
    m_deltaX = m_dstX - m_startX;
    m_deltaY = m_dstY - m_startY;
}