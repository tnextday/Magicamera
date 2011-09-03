#include "textureregion.h"
#include <stdio.h>
#include <string.h>

const int TRU1 = 0;
const int TRV1 = 1;
const int TRU2 = 2;
const int TRV2 = 3;
const int TRU3 = 4;
const int TRV3 = 5;
const int TRU4 = 6;
const int TRV4 = 7;

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

TextureRegion::TextureRegion( TextureRegion *tr )
{
    setTextureRegion(tr);
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

    m_texCoords[TRU1] = u;
    m_texCoords[TRV1] = v2;

    m_texCoords[TRU2] = u;
    m_texCoords[TRV2] = v;

    m_texCoords[TRU3] = u2;
    m_texCoords[TRV3] = v;

    m_texCoords[TRU4] = u2;
    m_texCoords[TRV4] = v2;
}

void TextureRegion::setRegion( Texture *tex )
{
    if (!tex) return;
    m_texture = tex;
    setRegion(0, 0, tex->getWidth(), tex->getHeight());
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
        m_texCoords[TRU1] = m_u;
        m_texCoords[TRU2] = m_u;
        m_texCoords[TRU3] = m_u2;
        m_texCoords[TRU4] = m_u2;
    }
    if (y) {
        float temp = m_v;
        m_v = m_v2;
        m_v2 = temp;
        m_texCoords[TRV1] = m_v2;
        m_texCoords[TRV2] = m_v;
        m_texCoords[TRV3] = m_v;
        m_texCoords[TRV4] = m_v2;
    }
}

void TextureRegion::setTexture( Texture *tex )
{
    m_texture = tex;
}

TextureRegion* TextureRegion::split( int rows, int cols)
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

    int tileHeight = height / rows;
    int tileWidth = width / cols;           

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
    return tiles;

}

TextureRegion* TextureRegion::split( Texture *tex, int rows, int cols)
{
    TextureRegion tr;
    tr.setRegion(tex);
    return tr.split(rows, cols);
}

void TextureRegion::rotate90( bool clockwise )
{
    if (clockwise) {
        float temp = m_texCoords[TRV1];
        m_texCoords[TRV1] = m_texCoords[TRV4];
        m_texCoords[TRV4] = m_texCoords[TRV3];
        m_texCoords[TRV3] = m_texCoords[TRV2];
        m_texCoords[TRV2] = temp;

        temp = m_texCoords[TRU1];
        m_texCoords[TRU1] = m_texCoords[TRU4];
        m_texCoords[TRU4] = m_texCoords[TRU3];
        m_texCoords[TRU3] = m_texCoords[TRU2];
        m_texCoords[TRU2] = temp;
    } else {
        float temp = m_texCoords[TRV1];
        m_texCoords[TRV1] = m_texCoords[TRV2];
        m_texCoords[TRV2] = m_texCoords[TRV3];
        m_texCoords[TRV3] = m_texCoords[TRV4];
        m_texCoords[TRV4] = temp;

        temp = m_texCoords[TRU1];
        m_texCoords[TRU1] = m_texCoords[TRU2];
        m_texCoords[TRU2] = m_texCoords[TRU3];
        m_texCoords[TRU3] = m_texCoords[TRU4];
        m_texCoords[TRU4] = temp;
    }
}


void TextureRegion::setTextureRegion( TextureRegion *tr )
{
    m_texture = tr->m_texture;
    m_u = tr->m_u;
    m_v = tr->m_v;
    m_u2 = tr->m_u2;
    m_v2 = tr->m_v2;
    memcpy(m_texCoords, tr->m_texCoords, sizeof(m_texCoords));
}