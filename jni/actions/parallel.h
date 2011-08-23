#pragma once
#include "action.h"
#include "utils/arraylist.h"

class Parallel :public Action
{
    ArrayList<Action *>     m_list;
    bool                    m_bDone;
public:
    Parallel(void);
    ~Parallel(void);

    virtual void setTarget( Sprite * val );

    virtual void update( float time );

    inline virtual bool isDone();

    virtual void step( float dt );

    Parallel* operator<<(Action *action);

    virtual void restart();
};
