#pragma once

class Action
{
private:
    int     m_tag;

protected:
    Sprite  *target;

public:
    Action(void);
    virtual ~Action(void);

    int getTag() const;
    void setTag(int val);

    Sprite * getTarget() const;
    void setTarget(Sprite * val);

    virtual bool isDone();

    //! called every frame with it's delta time. DON'T override unless you know what you are doing.
    virtual void step(float dt) = 0;

    //! called once per frame. time a value between 0 and 1
    //! For example: 
    //! * 0 means that the action just started
    //! * 0.5 means that the action is in the middle
    //! * 1 means that the action is over
    virtual void update(float time) = 0;
};
