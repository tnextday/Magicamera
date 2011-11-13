#pragma once
#include "glutils/sprite.h"
#include "utils/easing.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class Action
{
private:
    int     m_tag;

protected:
    Sprite  *m_target;
    float   m_duration;
    float   m_elapsed;
    CEasing  *m_easing;
    bool    m_firstTick;
public:
    bool    AutoFree;

public:
    Action(float duration);
    virtual ~Action(void);

    int getTag() const;
    void setTag(int val);

    Sprite * getTarget() const;
    virtual void setTarget(Sprite * val);

    Action* setEasing(CEasing *easing, bool autoFree = true);

    virtual void restart();

    virtual bool isDone(){return m_elapsed > m_duration;};

    //! called every frame with it's delta time. DON'T override unless you know what you are doing.
    virtual bool step(float dt);

    //! called once per frame. time a value between 0 and 1
    //! For example: 
    //! * 0 means that the action just started
    //! * 0.5 means that the action is in the middle
    //! * 1 means that the action is over
    virtual void update(float time) = 0;

protected:
    //call when first tick
    virtual void onStart();
};
