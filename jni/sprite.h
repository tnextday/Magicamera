#ifndef sprite_h__
#define sprite_h__

#include "Texture.h"

const int X1 = 0;
const int Y1 = 1;
const int C1 = 2;
const int U1 = 3;
const int V1 = 4;
const int X2 = 5;
const int Y2 = 6;
const int C2 = 7;
const int U2 = 8;
const int V2 = 9;
const int X3 = 10;
const int Y3 = 11;
const int C3 = 12;
const int U3 = 13;
const int V3 = 14;
const int X4 = 15;
const int Y4 = 16;
const int C4 = 17;
const int U4 = 18;
const int V4 = 19;

class Sprite : public Texture{
private:
    float u, v;
    float u2, v2;
    float x, y;
    float width, height;
    float originX, originY;
    float rotation;
    float scaleX, scaleY;
    bool dirty;
    float vertices[20];
public:
    Sprite();
    ~Sprite();
    void setPostion(float x, float y);
    void translate(float xAmount, float yAmount);
    void setOrigin(float originX, float originY);
    void setRotation (float degrees);
    void rotate (float degrees);
    void setScale (float scaleXY);
    void setScale (float scaleX, float scaleY);
    void scale (float amount);
    void draw();
    float* getVertices ();
    void flip (bool x, bool y);
    void rotate90 (bool clockwise);
};

#endif // sprite_h__
