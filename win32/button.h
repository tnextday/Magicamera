#pragma once
#include "baseshader.h"
#include "sprite.h"

class Button;

class ButtonClick{
public:
    virtual void onButtonClick(Button *btn) = 0;
};

class Button :public Sprite
{
private:
    TextureRegion   *m_down;
    ButtonClick     *m_onClick;
    bool            m_bShow;
    bool            m_bSelect;
    bool            m_bDown;
    int             m_tag;

public:
    Button(void);
    ~Button(void);

    void update(float delta);
    void draw(BaseShader *bs);

    void setOnClick(ButtonClick * val) { m_onClick = val; }
    bool selected() const { return m_bSelect; }
    void setSelected(bool val) { m_bSelect = val; }
    bool visible() const { return m_bShow; }
    void setVisible(bool val) { m_bShow = val; }
    int tag() const { return m_tag; }
    void setTag(int val) { m_tag = val; }
    bool onTouchDown(float x, float y);
    bool onTouchUp(float x, float y);
    
private:
    bool isContainPos(float x, float y);

};
