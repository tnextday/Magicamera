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

class Action;

class Sprite : public TextureRegion{
private:
    GLfloat     m_x, m_y;
    GLfloat     m_width, m_height;
    GLfloat     m_aspectRatio;
    GLfloat     m_originX, m_originY;
    GLfloat     m_rotation;
    GLfloat     m_scaleX, m_scaleY;
    bool        m_dirty;
    GLfloat     m_vertices[8];
    Action      *m_action;

protected:
    Texture     *m_spriteTexture;

public:
    Sprite();
    Sprite(const char* texPath);
    Sprite(const unsigned char* buffer, uint32_t size);
    Sprite(TextureRegion *textureRegion);
    Sprite(Texture *tex);
    Sprite(Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight);
    ~Sprite();
    void loadFromFile(const char* texPath);
    void loadFromMemory(const unsigned char* buffer, uint32_t size);
    void setTexture(Texture *tex);
    void setTexture(Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight);
    void setTextureRegion(TextureRegion *tr);

    void setPostion(GLfloat x, GLfloat y);
    void translate(GLfloat xAmount, GLfloat yAmount);
    void setOrigin(GLfloat originX, GLfloat originY);
    void setRotation (GLfloat degrees);
    float getRotation();
    void rotate (GLfloat degrees);
    void setScale (GLfloat scaleXY);
    void setScale (GLfloat scaleX, GLfloat scaleY);
    void setSize(GLfloat w, GLfloat h);
    void scale (GLfloat amount);
    void draw(BaseShader *shader, Texture *otherTex = NULL);
    //Vertices的排列顺序为(x1y1x2y2x3y3x4y4u1v1u2v2u3v3u4v4),不是交替排列的
    GLfloat* getVertices ();
    
    float getX(){return m_x;};
    float getY(){return m_y;};
    GLfloat getWidth() const { return m_width; }
    GLfloat getHeight() const { return m_height; }
    void getBoundingRect(rect_t &rect);
    void doAction(Action* action, bool autoFree = true);
    void update(float dt);
    bool isActionDone();

    
    //************************************
    // Method:    mapToWordSize
    // Description: 映射到世界坐标的大小，默认Sprite初始化会将宽初始化为1，高为宽高比
    //              调用此函数后会将宽高转化为纹理原大小相对于world size的大小                     
    // Parameter: float w 宽 
    // Parameter: float h 高 
    // Returns:   void
    //************************************
    void mapToWordSize(float w, float h);

private:
    void init(int srcX, int srcY, int srcWidth, int srcHeight);
};

#endif // sprite_h__
