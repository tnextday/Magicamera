#include "sequence.h"

Sequence::Sequence(void)
    :Action(0)
{
    m_bDone = false;
    m_currAction = 0;
}

Sequence::~Sequence(void)
{
    Action *action;
    for (int i = 0; i < m_list.size(); i++)
    {
        action = m_list[i];
        if (action && action->AutoFree)
            SafeDelete(action);
    }
    m_list.clear();
}

void Sequence::setTarget( Sprite * val )
{
    m_target = val;
    for (int i = 0; i < m_list.size(); i++)
    {
        m_list[i]->setTarget(m_target);
    }
}

void Sequence::update( float time )
{
}

bool Sequence::isDone()
{
    return m_bDone;
}

void Sequence::step( float dt )
{
    if (m_bDone) return;
    Action* action = m_list[m_currAction];
    action->step(dt);
    if (action->isDone()){
        m_currAction++;
        if (m_currAction == m_list.size()){
            m_bDone = true;
        }
    }
}

Sequence & Sequence::operator<<(Action *action )
{
    if (m_target)
        action->setTarget(m_target);
    m_list<<action;
    return *this;
}

void Sequence::restart()
{
    for (int i = 0; i < m_list.size(); i++)
    {
        m_list[i]->restart();
    }
    m_bDone = false;
}