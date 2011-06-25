#pragma once
#ifndef _textureregion_h_
#define _textureregion_h_

#include "texture.h"

class TextureRegion
{
protected:
    Texture *m_texture;
    float m_u, m_v;
    float m_u2, m_v2;
public:
    TextureRegion(void);
    TextureRegion(Texture *tex);
    TextureRegion(Texture *tex, float u, float v, float u2, float v2);
    ~TextureRegion(void);
    /**
    * @param width The width of the texture region. May be negative to flip the sprite when drawn.
    * @param height The height of the texture region. May be negative to flip the sprite when drawn.
    */
    void setRegioni(int x, int y, int width, int height);
    void setRegionf(float u, float v, float u2, float v2);
    void setRegion(Texture *tex);
    Texture* getTexture();
    void setTexture(Texture *tex);
    void flip(bool x, bool y);

};
#endif // _textureregion_h_
