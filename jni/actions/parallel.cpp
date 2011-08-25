#include "parallel.h"

Parallel::Parallel(void)
    :Action(0)
{
    m_list.ordered = false;
    m_bDone = false;
}

Parallel::~Parallel(void)
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

void Parallel::setTarget( Sprite * val )
{
    m_target = val;
    for (int i = 0; i < m_list.size(); i++)
    {
        m_list[i]->setTarget(m_target);
    }
}

void Parallel::update( float time )
{
}

bool Parallel::isDone()
{
    return m_bDone;
}

void Parallel::step( float dt )
{
    if (m_bDone) return;
    Action *action;
    m_bDone = true;
    for (int i = 0; i < m_list.size(); i++)
    {
        action = m_list[i];
        if (!action->isDone()){
            action->step(dt);
            m_bDone = false;
        }
    }
}

Parallel & Parallel::operator<<(Action *action )
{
    if (m_target)
        action->setTarget(m_target);
    m_list<<action;
    return *this;
}

void Parallel::restart()
{
    for (int i = 0; i < m_list.size(); i++)
    {
        m_list[i]->restart();
    }
    m_bDone = false;
}