#pragma once
#include "glutils/baseshader.h"
#include "glutils/sprite.h"
#include "ui/touchevent.h"

class Button;

class ButtonClick{
public:
    virtual void onButtonClick(Button *btn) = 0;
};

class Button :public Sprite, TouchEvent
{
private:
    TextureRegion   m_down;
    ButtonClick     *m_onClick;
    bool            m_bShow;
    bool            m_bSelect;
    bool            m_bDown;
    int             m_tag;
    Sprite*         m_btnImg;

public:
    //包含普通和按下状态的Texture,纵向排列
    Button(Texture *bgTex, Texture *imgTex = NULL, int tag = 0);
    Button(const char* bgTexPath, const char* imgPath = NULL, int tag = 0);
    //Button(TextureRegion *btnNormal, TextureRegion *btnDown);
    ~Button(void);

    //设置按钮背景，包含普通和按下状态的Texture,纵向排列
    void setBackground(Texture *bgTex);
    void loadBackground(const char* bgTexPath);
    //设置按钮图片
    void setImage(Texture *imgTex);
    void loadImage(const char* imgTexPath);

    void update(float delta);
    void draw(BaseShader *bs);

    void setOnClick(ButtonClick * val) { m_onClick = val; }
    bool selected() const { return m_bSelect; }
    void setSelected(bool val) { m_bSelect = val; }
    bool visible() const { return m_bShow; }
    void setVisible(bool val) { m_bShow = val; }
    int tag() const { return m_tag; }
    void setTag(int val) { m_tag = val; }

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchUp(float x, float y);
    virtual bool onTouchDrag( float x, float y );
    
private:
    bool isContainPos(float x, float y);

    
};
