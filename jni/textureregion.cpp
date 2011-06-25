#include "textureregion.h"

TextureRegion::TextureRegion(void)
{
}

TextureRegion::TextureRegion( Texture *tex )
{
    setRegion(tex);
}

TextureRegion::TextureRegion( Texture *tex, float u, float v, float u2, float v2 )
{
    m_texture = tex;
    setRegionf(u, v, u2, v2);
}
TextureRegion::~TextureRegion(void)
{
}

void TextureRegion::setRegioni( int x, int y, int width, int height )
{
    float invTexWidth = 1.0 / m_texture->getWidth();
    float invTexHeight = 1.0 / m_texture->getHeight();
    setRegionf(x * invTexWidth, y * invTexHeight, (x + width) * invTexWidth, (y + height) * invTexHeight);
}

void TextureRegion::setRegionf( float u, float v, float u2, float v2 )
{
    m_u = u;
    m_v = v;
    m_u2 = u2;
    m_v2 = v2;
}

void TextureRegion::setRegion( Texture *tex )
{
    m_texture = tex;
    setRegioni(0, 0, tex->getWidth(), tex->getWidth());
}

Texture* TextureRegion::getTexture()
{
    return m_texture;
}

void TextureRegion::flip( bool x, bool y )
{
    if (x) {
        float temp = m_u;
        m_u = m_u2;
        m_u2 = temp;
    }
    if (y) {
        float temp = m_v;
        m_v = m_v2;
        m_v2 = temp;
    }	
}

void TextureRegion::setTexture( Texture *tex )
{
    m_texture = tex;
}