#include "button.h"

Button::Button(void)
{
    m_status = ButtonNormal;
}

Button::~Button(void)
{
}

void Button::update( float delta )
{

}

void Button::draw( BaseShader *bs )
{
    switch (m_status){
        case ButtonDown:
            break;
        case ButtonSelected:
            break;
        default:    //ButtonNormal
            ;
    }
}