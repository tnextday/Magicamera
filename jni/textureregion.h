#pragma once
#ifndef _textureregion_h_
#define _textureregion_h_

#include "texture.h"

const int U1 = 0;
const int V1 = 1;
const int U2 = 2;
const int V2 = 3;
const int U3 = 4;
const int V3 = 5;
const int U4 = 6;
const int V4 = 7;

class TextureRegion
{
protected:
    Texture *m_texture;
    float m_u, m_v;
    float m_u2, m_v2;
    GLfloat     m_texCoords[8];
public:
    TextureRegion(void);
    TextureRegion(Texture *tex);
    TextureRegion(Texture *tex, float u, float v, float u2, float v2);
    TextureRegion(Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight);
    ~TextureRegion(void);
    /**
    * @param width The width of the texture region. May be negative to flip the sprite when drawn.
    * @param height The height of the texture region. May be negative to flip the sprite when drawn.
    */
    void setRegion(int x, int y, int width, int height);
    void setRegion(float u, float v, float u2, float v2);
    void setRegion(Texture *tex);
    int getRegionX ();
    int getRegionY ();
    int getRegionWidth ();
    int getRegionHeight ();
    Texture* getTexture();
    void setTexture(Texture *tex);
    void flip(bool x, bool y);
    void rotate90 (bool clockwise);

    void copy(TextureRegion *tr);

    GLfloat* getTexCoords();
    
    //返回一个TextureRegion数组，数组长度为count
    TextureRegion* split(int tileWidth, int tileHeight, int *count);
    static TextureRegion* split(Texture *tex, int tileWidth, int tileHeight, int *count);
};
#endif // _textureregion_h_
