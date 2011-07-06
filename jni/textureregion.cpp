#include "textureregion.h"
#include <stdio.h>
#include <string.h>

TextureRegion::TextureRegion(void)
{
    m_texture = NULL;
}

TextureRegion::TextureRegion( Texture *tex )
{
    setRegion(tex);
}

TextureRegion::TextureRegion( Texture *tex, float u, float v, float u2, float v2 )
{
    m_texture = tex;
    setRegion(u, v, u2, v2);
}

TextureRegion::TextureRegion( Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight )
{
    m_texture = tex;
    setRegion(srcX, srcY, srcWidth, srcHeight);
}
TextureRegion::~TextureRegion(void)
{
}

void TextureRegion::setRegion( int x, int y, int width, int height )
{
    float invTexWidth = 1.0 / m_texture->getWidth();
    float invTexHeight = 1.0 / m_texture->getHeight();
    setRegion(x * invTexWidth, y * invTexHeight, (x + width) * invTexWidth, (y + height) * invTexHeight);
}

void TextureRegion::setRegion( float u, float v, float u2, float v2 )
{
    m_u = u;
    m_v = v;
    m_u2 = u2;
    m_v2 = v2;

    m_texCoords[U1] = u;
    m_texCoords[V1] = v2;

    m_texCoords[U2] = u;
    m_texCoords[V2] = v;

    m_texCoords[U3] = u2;
    m_texCoords[V3] = v;

    m_texCoords[U4] = u2;
    m_texCoords[V4] = v2;
}

void TextureRegion::setRegion( Texture *tex )
{
    m_texture = tex;
    setRegion(0, 0, tex->getWidth(), tex->getWidth());
}

Texture* TextureRegion::getTexture()
{
    return m_texture;
}

int TextureRegion::getRegionX () {
        return (int)(m_u * m_texture->getWidth());
}

int TextureRegion::getRegionY () {
        return (int)(m_v * m_texture->getHeight());
}


/**
 * Returns the region's width. May be negative if the texture region is flipped horizontally.
 */
int TextureRegion::getRegionWidth () {
        return (int)((m_u2 - m_u) * m_texture->getWidth());
}

/**
 * Returns the region's height. May be negative if the texture region is flipped horizontally.
 */
int TextureRegion::getRegionHeight () {
        return (int)((m_v2 - m_v) * m_texture->getHeight());
}

void TextureRegion::flip( bool x, bool y )
{
    if (x) {
        float temp = m_u;
        m_u = m_u2;
        m_u2 = temp;
        m_texCoords[U1] = m_u2;
        m_texCoords[U2] = m_u2;
        m_texCoords[U3] = m_u;
        m_texCoords[U4] = m_u;
    }
    if (y) {
        float temp = m_v;
        m_v = m_v2;
        m_v2 = temp;
        m_texCoords[V1] = m_v;
        m_texCoords[V2] = m_v2;
        m_texCoords[V3] = m_v2;
        m_texCoords[V4] = m_v;
    }
}

void TextureRegion::setTexture( Texture *tex )
{
    m_texture = tex;
}

TextureRegion* TextureRegion::split( int tileWidth, int tileHeight, int *count)
{
    int x = getRegionX();
    int y = getRegionY();
    int width = getRegionWidth();
    int height = getRegionHeight();

    if(width < 0) {
        x = x - width;
        width = -width;
    }

    if(height < 0) {
        y = y - height;
        height = -height;
    }

    int rows = height / tileHeight;
    int cols = width / tileWidth;           

    int startX = x;
    int idx = 0;
    TextureRegion *tiles = new TextureRegion[rows*cols];                
    for(int row = 0; row < rows; row++, y += tileHeight) {
        x = startX;
        for(int col = 0; col < cols; col++, x += tileWidth) {
            tiles[idx].setTexture(m_texture);
            tiles[idx].setRegion(x, y, tileWidth, tileHeight);
            idx++;
        }
    }
    *count =  rows*cols;
    return tiles;

}

TextureRegion* TextureRegion::split( Texture *tex, int tileWidth, int tileHeight, int *count)
{
    TextureRegion tr;
    tr.setRegion(tex);
    return tr.split(tileWidth, tileHeight, count);
}

void TextureRegion::rotate90( bool clockwise )
{
    if (clockwise) {
        float temp = m_texCoords[V1];
        m_texCoords[V1] = m_texCoords[V4];
        m_texCoords[V4] = m_texCoords[V3];
        m_texCoords[V3] = m_texCoords[V2];
        m_texCoords[V2] = temp;

        temp = m_texCoords[U1];
        m_texCoords[U1] = m_texCoords[U4];
        m_texCoords[U4] = m_texCoords[U3];
        m_texCoords[U3] = m_texCoords[U2];
        m_texCoords[U2] = temp;
    } else {
        float temp = m_texCoords[V1];
        m_texCoords[V1] = m_texCoords[V2];
        m_texCoords[V2] = m_texCoords[V3];
        m_texCoords[V3] = m_texCoords[V4];
        m_texCoords[V4] = temp;

        temp = m_texCoords[U1];
        m_texCoords[U1] = m_texCoords[U2];
        m_texCoords[U2] = m_texCoords[U3];
        m_texCoords[U3] = m_texCoords[U4];
        m_texCoords[U4] = temp;
    }
}

GLfloat* TextureRegion::getTexCoords()
{
    return m_texCoords;
}

void TextureRegion::copy( TextureRegion *tr )
{
    m_texture = tr->m_texture;
    m_u = tr->m_u;
    m_v = tr->m_v;
    m_u2 = tr->m_u2;
    m_v2 = tr->m_v2;
    memcpy(m_texCoords, tr->m_texCoords, sizeof(m_texCoords));
}