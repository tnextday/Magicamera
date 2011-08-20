#include "action.h"
#include <math.h>

Action::Action(float duration)
{
    m_duration = duration;
    m_elapsed = 0;
    m_easing = NULL;
    AutoFree = true;
}

Action::~Action(void)
{
    if (m_easing && m_easing->AutoFree){
        SafeDelete(m_easing);
    }
}

int Action::getTag() const
{
    return m_tag;
}

void Action::setTag( int val )
{
    m_tag = val;
}

Sprite * Action::getTarget() const
{
    return m_target;
}

void Action::setTarget( Sprite * val )
{
    m_target = val;
}

void Action::start( float duration )
{
    m_duration = duration;
    m_elapsed = 0;
}

void Action::step( float dt )
{
    if (m_elapsed < 0)
        m_elapsed = 0;
    m_elapsed += dt;
    if (m_easing){
        update(m_easing->update(min(1, m_elapsed/m_duration)));
    }else{
        update(min(1, m_elapsed/m_duration));
    }
    
}

bool Action::isDone()
{
    return m_elapsed > m_duration;
}

Action* Action::setEasing( CEasing *easing, bool autoFree /*= true*/)
{
    if (m_easing && m_easing->AutoFree){
        SafeDelete(m_easing);
    }
    m_easing = easing;
    m_easing->AutoFree = autoFree;
    return this;
}