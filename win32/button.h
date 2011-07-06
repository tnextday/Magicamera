#pragma once
#include "baseshader.h"
#include "sprite.h"

enum ButtonStatus{
    ButtonNormal,
    ButtonDown,
    ButtonSelected
};

class Button :public Sprite
{
    ButtonStatus    m_status;
    TextureRegion   m_selected;
    TextureRegion   m_down;
public:
    Button(void);
    ~Button(void);

    void update(float delta);
    void draw(BaseShader *bs);
};
