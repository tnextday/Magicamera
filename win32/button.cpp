#include "button.h"

Button::Button(void)
{
    m_onClick = NULL;
    m_down = NULL;
    m_bDown = false;
    m_bSelect = false;
}

Button::~Button(void)
{
    delete m_down;
}

void Button::update( float delta )
{

}

void Button::draw( BaseShader *bs )
{
    TextureRegion *tr = this;
    if (m_bDown || m_bSelect && m_down){
        tr = m_down;
    }
    tr->getTexture()->bind();
    glVertexAttribPointer(bs->getPositionLoc(), 2, GL_FLOAT, GL_FALSE, 0, getVertices());
    glVertexAttribPointer(bs->getTextureCoordLoc(), 2, GL_FLOAT, GL_FALSE, 0, tr->getTexCoords());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
}

bool Button::onTouchDown( float x, float y )
{
    m_bDown = m_bShow && isContainPos(x, y);
    return true;
}

bool Button::onTouchUp( float x, float y )
{
    if (m_bDown && m_bShow && isContainPos(x, y) && m_onClick){
        m_onClick->onButtonClick(this);
    }
    m_bDown = false;
    return true;
}

bool Button::isContainPos( float x, float y )
{
    rect_t r;
    getBoundingRect(r);
    return x >= r.x && x <= r.width && y >= r.y && y <= r.height;
}