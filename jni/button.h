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
    TextureRegion   m_down;
    ButtonClick     *m_onClick;
    bool            m_bShow;
    bool            m_bSelect;
    bool            m_bDown;
    int             m_tag;
    Texture         *m_btnTexture;

public:
    //包含普通和按下状态的Texture,纵向排列
    Button(Texture *tex, int tag = 0);
    Button(const char* btnTexPath, int tag = 0);
    //Button(TextureRegion *btnNormal, TextureRegion *btnDown);
    ~Button(void);

    void setTexture(Texture *tex);

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
