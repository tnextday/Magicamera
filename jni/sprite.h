#ifndef sprite_h__
#define sprite_h__

#include "Texture.h"
#include "textureregion.h"
#include <GLES2/gl2.h>

const int X1 = 0;
const int Y1 = 1;
const int U1 = 2;
const int V1 = 3;
const int X2 = 4;
const int Y2 = 5;
const int U2 = 6;
const int V2 = 7;
const int X3 = 8;
const int Y3 = 9;
const int U3 = 10;
const int V3 = 11;
const int X4 = 12;
const int Y4 = 13;
const int U4 = 14;
const int V4 = 15;

struct rect_t{
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
};

class Sprite : public TextureRegion{
private:
    GLfloat     m_u, m_v;
    GLfloat     m_u2, m_v2;
    GLfloat     m_x, m_y;
    GLfloat     m_width, m_height;
    GLfloat     m_originX, m_originY;
    GLfloat     m_rotation;
    GLfloat     m_scaleX, m_scaleY;
    bool        m_dirty;
    GLfloat     m_vertices[16];
    GLuint      m_positionLoc;
    GLuint      m_texCoordLoc;
public:
    Sprite();
    ~Sprite();
    void setPostion(GLfloat x, GLfloat y);
    void translate(GLfloat xAmount, GLfloat yAmount);
    void setOrigin(GLfloat originX, GLfloat originY);
    void setRotation (GLfloat degrees);
    void rotate (GLfloat degrees);
    void setScale (GLfloat scaleXY);
    void setScale (GLfloat scaleX, GLfloat scaleY);
    void scale (GLfloat amount);
    void draw();
    GLfloat* getVertices ();
    void flip (bool x, bool y);
    void rotate90 (bool clockwise);
    float getX();
    float getY();
    void getBoundingRect(rect_t &rect);
};

#endif // sprite_h__
