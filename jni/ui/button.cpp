#include "button.h"


Button::Button( Texture *bgTex, Texture *imgTex /*= NULL*/, int tag /*= 0*/)
    :Sprite()
{
    m_onClick = NULL;
    m_btnImg = NULL;
    m_bDown = false;
    m_bSelect = false;
    m_bShow = true;
    m_tag = tag;
    setBackground(bgTex);
    if (imgTex){
        setImage(imgTex);
    }
}

Button::Button( const char* bgTexPath, const char* imgPath /*= NULL*/, int tag /*= 0*/ )
    :Sprite()
{
    m_onClick = NULL;
    m_btnImg = NULL;
    m_bDown = false;
    m_bSelect = false;
    m_bShow = true;
    m_tag = tag;
    loadBackground(bgTexPath);
    if (imgPath){
        loadImage(imgPath);
    }
}
Button::~Button(void)
{
    SafeDelete(m_btnImg);
}

void Button::update( float delta )
{

}

void Button::draw( BaseShader *bs )
{
    if (!m_bShow) return;
    TextureRegion *tr = this;
    if (m_bDown || m_bSelect){
        tr = &m_down;
    }
    tr->getTexture()->bind();
    glVertexAttribPointer(bs->getPositionLoc(), 2, GL_FLOAT, GL_FALSE, 0, getVertices());
    glVertexAttribPointer(bs->getTextureCoordLoc(), 2, GL_FLOAT, GL_FALSE, 0, tr->getTexCoords());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    if (m_btnImg){
        m_btnImg->setPostion(getX(), getY());
        m_btnImg->draw(bs);
    }
    
}

bool Button::isContainPos( float x, float y )
{
    rect_t r;
    getBoundingRect(r);
    return x >= r.x && x <= r.x+r.width && y >= r.y && y <= r.y+r.height;
}

void Button::setBackground( Texture *bgTex )
{
    TextureRegion* btnTexs = TextureRegion::split(bgTex, 2, 1);
    setTextureRegion(&btnTexs[0]);
    m_down.setTextureRegion(&btnTexs[1]);
    delete[] btnTexs;
}

void Button::loadBackground( const char* bgTexPath )
{
    m_spriteTexture = new Texture(bgTexPath);
    setBackground(m_spriteTexture);
}

bool Button::onTouchDrag( float x, float y )
{
    return false;
}

bool Button::onTouchDown( float x, float y )
{
    m_bDown = m_bShow && isContainPos(x, y);
    return m_bDown;
}

bool Button::onTouchUp( float x, float y )
{
    bool bTouch = m_bShow && isContainPos(x, y);
    if (m_bDown && bTouch && m_onClick){
        m_onClick->onButtonClick(this);
    }
    m_bDown = false;
    return bTouch;
}

void Button::setImage( Texture *imgTex )
{
    SafeDelete(m_btnImg);
    m_btnImg = new Sprite(imgTex);
}

void Button::loadImage( const char* imgTexPath )
{
    SafeDelete(m_btnImg);
    m_btnImg = new Sprite(imgTexPath);
}