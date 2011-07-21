#ifndef _touchevent_h_
#define _touchevent_h_

class TouchEvent{
public:
    virtual bool onTouchDown(float x, float y) = 0;
    virtual bool onTouchUp(float x, float y) = 0;
    virtual bool onTouchDrag(float x, float y) = 0;
};

#endif // _touchevent_h_
