#ifndef sprite_h__
#define sprite_h__

#include "Texture.h"
#include "textureregion.h"
#include <GLES2/gl2.h>
#include "baseshader.h"

const int X1 = 0;
const int Y1 = 1;
const int X2 = 2;
const int Y2 = 3;
const int X3 = 4;
const int Y3 = 5;
const int X4 = 6;
const int Y4 = 7;


struct rect_t{
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
};

class Sprite : public TextureRegion{
private:
    GLfloat     m_x, m_y;
    GLfloat     m_width, m_height;
    GLfloat     m_originX, m_originY;
    GLfloat     m_rotation;
    GLfloat     m_scaleX, m_scaleY;
    bool        m_dirty;
    GLfloat     m_vertices[8];
public:
    Sprite();
    Sprite(Texture *tex);
    Sprite(Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight);
    ~Sprite();
    void setTexture(Texture *tex);
    void setTexture(Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight);

    void setPostion(GLfloat x, GLfloat y);
    void translate(GLfloat xAmount, GLfloat yAmount);
    void setOrigin(GLfloat originX, GLfloat originY);
    void setRotation (GLfloat degrees);
    void rotate (GLfloat degrees);
    void setScale (GLfloat scaleXY);
    void setScale (GLfloat scaleX, GLfloat scaleY);
    void setSize(GLfloat w, GLfloat h);
    void scale (GLfloat amount);
    void draw(BaseShader *shader);
    //Vertices的排列顺序为(x1y1x2y2x3y3x4y4u1v1u2v2u3v3u4v4),不是交替排列的
    GLfloat* getVertices ();
    
    float getX();
    float getY();
    void getBoundingRect(rect_t &rect);

private:
    void init(int srcX, int srcY, int srcWidth, int srcHeight);
};

#endif // sprite_h__
