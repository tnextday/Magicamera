#include "action.h"

Action::Action(void)
{
}

Action::~Action(void)
{
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
    return target;
}

void Action::setTarget( Sprite * val )
{
    target = val;
}