#ifndef sprite_h__
#define sprite_h__

#include "Texture.h"

class Sprite : public Texture{
public:
    void draw();
    void setPostion(float x, float y);
    void translate(float xAmount, float yAmount);
    void setOrigin(float originX, float originY);
    void setRotation (float degrees);
    void rotate (float degrees);
    void setScale (float scaleXY);
    void setScale (float scaleX, float scaleY);
    void scale (float amount);
};

#endif // sprite_h__
