#include "button.h"


Button::Button( Texture *tex, int tag /*= 0*/)
{
    m_onClick = NULL;
    m_btnTexture = NULL; 
    m_bDown = false;
    m_bSelect = false;
    m_bShow = true;
    m_tag = tag;
    setTexture(m_btnTexture);
}

Button::Button( const char* btnTexPath, int tag /*= 0*/ )
{
    m_onClick = NULL;
    m_bDown = false;
    m_bSelect = false;
    m_bShow = true;
    m_tag = tag;
    m_btnTexture = new Texture(btnTexPath);
    setTexture(m_btnTexture);
}

Button::~Button(void)
{
    SafeDelete(m_btnTexture);
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
    
}

bool Button::isContainPos( float x, float y )
{
    rect_t r;
    getBoundingRect(r);
    return x >= r.x && x <= r.x+r.width && y >= r.y && y <= r.y+r.height;
}

void Button::setTexture( Texture *tex )
{
    TextureRegion* btnTexs = TextureRegion::split(tex, 2, 1);
    setTextureRegion(&btnTexs[0]);
    m_down.setTextureRegion(&btnTexs[1]);
    delete[] btnTexs;

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