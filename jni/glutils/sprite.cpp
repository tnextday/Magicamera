#include "sprite.h"
#include "utils/fastmath.h"
#include <math.h>
#include <stdlib.h>
#include "actions/action.h"

Sprite::Sprite()
    :TextureRegion()
{
    m_spriteTexture = NULL;
    m_dirty = true;
    m_action = NULL;
}

Sprite::Sprite( Texture *tex )
    :TextureRegion(tex)
{
    m_spriteTexture = NULL;
    m_action = NULL;
    setTexture(tex);
}

Sprite::Sprite( Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight)
    :TextureRegion(tex, srcX, srcY, srcWidth, srcHeight)
{
    m_spriteTexture = NULL;
    m_action = NULL;
    setTexture(tex, srcX, srcY, srcWidth, srcHeight);
}

Sprite::Sprite( TextureRegion *textureRegion )
        :TextureRegion(textureRegion)
{
    m_action = NULL;
    m_spriteTexture = NULL;
    m_dirty = true;
}

Sprite::Sprite( const char* texPath )
:TextureRegion()
{
    m_dirty = true;
    m_action = NULL;
    loadFromFile(texPath);
}

Sprite::Sprite( const unsigned char* buffer, uint32_t size )
:TextureRegion()
{
    m_dirty = true;
    m_action = NULL;
    loadFromMemory(buffer, size);
}
Sprite::~Sprite()
{
    if (m_action && m_action->AutoFree){
        SafeDelete(m_action);
    }
    SafeDelete(m_spriteTexture);
}

float* Sprite::getVertices()
{
    if (m_dirty) {
        m_dirty = false;

        float localX = -m_originX;
        float localY = -m_originY;
        float localX2 = localX + m_width;
        float localY2 = localY + m_height;
        if (m_scaleX != 1 || m_scaleY != 1) {
            localX *= m_scaleX;
            localY *= m_scaleY;
            localX2 *= m_scaleX;
            localY2 *= m_scaleY;
        }
        if (m_rotation != 0) {
            float cosv,sinv;
            cosv = fastcosDeg(m_rotation);
            sinv = fastsinDeg(m_rotation);
            float localXCos = localX * cosv;
            float localXSin = localX * sinv;
            float localYCos = localY * cosv;
            float localYSin = localY * sinv;
            float localX2Cos = localX2 * cosv;
            float localX2Sin = localX2 * sinv;
            float localY2Cos = localY2 * cosv;
            float localY2Sin = localY2 * sinv;

            float x1 = localXCos - localYSin + m_x;
            float y1 = localYCos + localXSin + m_y;
            m_vertices[X1] = x1;
            m_vertices[Y1] = y1;

            float x2 = localXCos - localY2Sin + m_x;
            float y2 = localY2Cos + localXSin + m_y;
            m_vertices[X2] = x2;
            m_vertices[Y2] = y2;

            float x3 = localX2Cos - localY2Sin + m_x;
            float y3 = localY2Cos + localX2Sin + m_y;
            m_vertices[X3] = x3;
            m_vertices[Y3] = y3;

            m_vertices[X4] = x1 + (x3 - x2);
            m_vertices[Y4] = y3 - (y2 - y1);
        } else {
            float x1 = localX + m_x;
            float y1 = localY + m_y;
            float x2 = localX2 + m_x;
            float y2 = localY2 + m_y;

            m_vertices[X1] = x1;
            m_vertices[Y1] = y1;

            m_vertices[X2] = x1;
            m_vertices[Y2] = y2;

            m_vertices[X3] = x2;
            m_vertices[Y3] = y2;

            m_vertices[X4] = x2;
            m_vertices[Y4] = y1;
        }
    }
    return m_vertices;
}

void Sprite::translate( float xAmount, float yAmount )
{
    if (xAmount == 0 && yAmount == 0) return;
    m_x += xAmount;
    m_y += yAmount;

    if (m_dirty) return;

    m_vertices[X1] += xAmount;
    m_vertices[Y1] += yAmount;

    m_vertices[X2] += xAmount;
    m_vertices[Y2] += yAmount;

    m_vertices[X3] += xAmount;
    m_vertices[Y3] += yAmount;

    m_vertices[X4] += xAmount;
    m_vertices[Y4] += yAmount;
}

void Sprite::setOrigin( float originX, float originY )
{
    if (m_originX == originX && m_originY == originY) return;
    m_originX = originX;
    m_originY = originY;
    m_dirty = true;
}

void Sprite::setRotation( float degrees )
{
    if (m_rotation == degrees) return;
    m_rotation = degrees;
    m_dirty = true;
}

void Sprite::rotate( float degrees )
{
    if (degrees == 0) return;
    m_rotation += degrees;
    m_dirty = true;
}

void Sprite::setScale( float scaleXY )
{
    if (m_scaleX == scaleXY && m_scaleY == scaleXY) return;
    m_scaleX = scaleXY;
    m_scaleY = scaleXY;
    m_dirty = true;
}

void Sprite::setScale( float scaleX, float scaleY )
{
    if (m_scaleX == scaleX && m_scaleY == scaleY) return;
    m_scaleX = scaleX;
    m_scaleY = scaleY;
    m_dirty = true;
}

void Sprite::scale( float amount )
{
    if (amount == 0) return;
    m_scaleX += amount;
    m_scaleY += amount;
    m_dirty = true;
}


void Sprite::draw(BaseShader *shader, Texture *otherTex /*= NULL*/)
{
    Texture *dst = NULL;
    if (otherTex){
        dst = otherTex;
    }else if (m_texture){
        dst = m_texture;
    }
    if (!dst) return;
    shader->use();
    dst->bind();
    glEnableVertexAttribArray(shader->getPositionLoc());
    glEnableVertexAttribArray(shader->getTextureCoordLoc());
    glVertexAttribPointer(shader->getPositionLoc(), 2, GL_FLOAT, GL_FALSE, 0, getVertices());
    glVertexAttribPointer(shader->getTextureCoordLoc(), 2, GL_FLOAT, GL_FALSE, 0, getTexCoords());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Sprite::setPostion( float x, float y )
{
    translate(x - m_x, y - m_y);
}

void Sprite::getBoundingRect( rect_t &rect )
{
    float minx = m_vertices[X1];
    float miny = m_vertices[Y1];
    float maxx = m_vertices[X1];
    float maxy = m_vertices[Y1];

    minx = minx > m_vertices[X2]? m_vertices[X2]: minx;
    minx = minx > m_vertices[X3]? m_vertices[X3]: minx;
    minx = minx > m_vertices[X4]? m_vertices[X4]: minx;

    maxx = maxx < m_vertices[X2]? m_vertices[X2]: maxx;
    maxx = maxx < m_vertices[X3]? m_vertices[X3]: maxx;
    maxx = maxx < m_vertices[X4]? m_vertices[X4]: maxx;

    miny = miny > m_vertices[Y2]? m_vertices[Y2]: miny;
    miny = miny > m_vertices[Y3]? m_vertices[Y3]: miny;
    miny = miny > m_vertices[Y4]? m_vertices[Y4]: miny;

    maxy = maxy < m_vertices[Y2]? m_vertices[Y2]: maxy;
    maxy = maxy < m_vertices[Y3]? m_vertices[Y3]: maxy;
    maxy = maxy < m_vertices[Y4]? m_vertices[Y4]: maxy;

    rect.x = minx;
    rect.y = miny;
    rect.width = maxx - minx;
    rect.height = maxy - miny;
}


void Sprite::init(int srcX, int srcY, int srcWidth, int srcHeight)
{
    m_scaleX = 1;
    m_scaleY = 1;
    m_dirty = true;
    m_rotation = 0;
    m_x = 0;
    m_y = 0;
    m_action = NULL;
    setRegion(srcX, srcY, srcWidth, srcHeight);
    setSize(abs(srcWidth), abs(srcHeight));
    setOrigin(m_width / 2, m_height / 2);
}


void Sprite::setSize( GLfloat w, GLfloat h )
{
    if(m_aspectRatio == w/h) return;
    m_aspectRatio = w/h;
    m_width = m_aspectRatio;
    m_height = 1.0;

    if (m_dirty) return;

    float x2 = m_x + m_width;
    float y2 = m_y + m_height;
    m_vertices[X1] = m_x;
    m_vertices[Y1] = m_y;

    m_vertices[X2] = m_x;
    m_vertices[Y2] = y2;

    m_vertices[X3] = x2;
    m_vertices[Y3] = y2;

    m_vertices[X4] = x2;
    m_vertices[Y4] = m_y;

    if (m_rotation != 0 || m_scaleX != 1 || m_scaleY != 1) m_dirty = true;
}

void Sprite::setTexture( Texture *tex )
{
    if (!tex) return;
    setTexture(tex, 0,0,tex->getWidth(), tex->getHeight());
}

void Sprite::setTexture( Texture *tex, int srcX, int srcY, int srcWidth, int srcHeight )
{
    if (!tex) return;
    m_texture = tex;
    init(srcX, srcY, srcWidth, srcHeight);
}

void Sprite::setTextureRegion( TextureRegion *tr )
{
    TextureRegion::setTextureRegion(tr);
    init(tr->getRegionX(), tr->getRegionY(), tr->getRegionWidth(), tr->getRegionHeight());
}


void Sprite::doAction( Action* action, bool autoFree /*= true*/)
{
    if (m_action && m_action->AutoFree){
        SafeDelete(m_action);
    }
    m_action = action;
    m_action->AutoFree = autoFree;
    m_action->setTarget(this);
}

bool Sprite::update( float dt )
{
    bool bUpdated = false;
    if (m_action){
        bUpdated = m_action->step(dt);
    }
    return bUpdated;
}

bool Sprite::isActionDone()
{
     if (m_action){
         return m_action->isDone();
     }
     return false;
}

float Sprite::getRotation()
{
    return m_rotation;
}

void Sprite::loadFromMemory( const unsigned char* buffer, uint32_t size )
{
    m_spriteTexture = new Texture(buffer, size);
    setTexture(m_spriteTexture);
}


void Sprite::loadFromFile( const char* texPath )
{
    m_spriteTexture = new Texture(texPath);
    setTexture(m_spriteTexture);
}

void Sprite::mapToWordSize( float w, float h )
{
    m_height = getRegionHeight()/h;
    m_width = m_height*m_aspectRatio;

    if (m_dirty) return;

    float x2 = m_x + m_width;
    float y2 = m_y + m_height;
    m_vertices[X1] = m_x;
    m_vertices[Y1] = m_y;

    m_vertices[X2] = m_x;
    m_vertices[Y2] = y2;

    m_vertices[X3] = x2;
    m_vertices[Y3] = y2;

    m_vertices[X4] = x2;
    m_vertices[Y4] = m_y;

    if (m_rotation != 0 || m_scaleX != 1 || m_scaleY != 1) m_dirty = true;
}