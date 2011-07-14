#pragma once
#include "baseshader.h"
#include "sprite.h"

enum ButtonStatus{
    ButtonNormal,
    ButtonDown
};

class ButtonClick{
public:
    virtual void onButtonClick(Button *btn) = 0;
};

class Button :public Sprite
{
    ButtonStatus    m_status;
    TextureRegion   m_down;
    ButtonClick     *m_onClick;
public:
    int             tag;

public:
    Button(void);
    ~Button(void);

    void update(float delta);
    void draw(BaseShader *bs);

    bool onTouchDown(float x, float y);
    bool onTouchDrag(float x, float y);
    bool onTouchUp(float x, float y);
};
