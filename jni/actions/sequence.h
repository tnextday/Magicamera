#pragma once
#include "action.h"
#include "utils/arraylist.h"

class Sequence :public Action
{
    ArrayList<Action *>     m_list;
    bool                    m_bDone;
    int                     m_currAction;
public:
    Sequence(void);
    ~Sequence(void);

    virtual void setTarget( Sprite * val );

    virtual void update( float time );

    inline virtual bool isDone();

    virtual void step( float dt );

    Sequence* operator<<(Action *action);
};
