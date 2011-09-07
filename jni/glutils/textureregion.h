#pragma once
#ifndef _textureregion_h_
#define _textureregion_h_

#include "texture.h"
#include <stdlib.h>

class TextureRegion
{
protected:
    Texture *m_texture;
    float m_u, m_v;
    float m_u2, m_v2;
    GLfloat     m_texCoords[8];
public:
    TextureRegion(void);
    TextureRegion(TextureRegion *tr);
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

    void setTextureRegion(TextureRegion *tr);

    bool isAvailable(){return m_texture != NULL;};

    GLfloat* getTexCoords(){ return m_texCoords; };
    
    //返回一个TextureRegion数组，数组长度为count
    TextureRegion* split(int rows, int cols);
    static TextureRegion* split(Texture *tex, int rows, int cols);
};
#endif // _textureregion_h_
