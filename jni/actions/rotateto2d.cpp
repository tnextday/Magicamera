#include "rotateto2d.h"

RotateTo2D::RotateTo2D(float ang, float duration)
        :Action(duration)
{
    m_dstAngle = ang;
}

void RotateTo2D::update( float time )
{
    m_target->setRotation(m_startAngle + m_deltaAngle*time);
}

void RotateTo2D::onStart()
{
    m_startAngle = m_target->getRotation();
    m_deltaAngle = m_dstAngle - m_startAngle;
}