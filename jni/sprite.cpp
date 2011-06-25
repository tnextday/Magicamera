#include "sprite.h"

Sprite::Sprite()
{
    m_scaleX = 0;
    m_scaleY = 0;
    m_dirty = true;
}

float* Sprite::getVertices()
{
    if (m_dirty) {
        m_dirty = false;

        float localX = -m_originX;
        float localY = -m_originY;
        float localX2 = localX + m_width;
        float localY2 = localY + m_height;
        float worldOriginX = m_x - localX;
        float worldOriginY = m_y - localY;
        if (m_scaleX != 1 || m_scaleY != 1) {
            localX *= m_scaleX;
            localY *= m_scaleY;
            localX2 *= m_scaleX;
            localY2 *= m_scaleY;
        }
        if (m_rotation != 0) {
            float cos,sin;
//             cos = MathUtils.cosDeg(rotation);
//             sin = MathUtils.sinDeg(rotation);
            //TODO 写一个快速的三角函数计算库
            float localXCos = localX * cos;
            float localXSin = localX * sin;
            float localYCos = localY * cos;
            float localYSin = localY * sin;
            float localX2Cos = localX2 * cos;
            float localX2Sin = localX2 * sin;
            float localY2Cos = localY2 * cos;
            float localY2Sin = localY2 * sin;

            float x1 = localXCos - localYSin + worldOriginX;
            float y1 = localYCos + localXSin + worldOriginY;
            m_vertices[X1] = x1;
            m_vertices[Y1] = y1;

            float x2 = localXCos - localY2Sin + worldOriginX;
            float y2 = localY2Cos + localXSin + worldOriginY;
            m_vertices[X2] = x2;
            m_vertices[Y2] = y2;

            float x3 = localX2Cos - localY2Sin + worldOriginX;
            float y3 = localY2Cos + localX2Sin + worldOriginY;
            m_vertices[X3] = x3;
            m_vertices[Y3] = y3;

            m_vertices[X4] = x1 + (x3 - x2);
            m_vertices[Y4] = y3 - (y2 - y1);
        } else {
            float x1 = localX + worldOriginX;
            float y1 = localY + worldOriginY;
            float x2 = localX2 + worldOriginX;
            float y2 = localY2 + worldOriginY;

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

void Sprite::flip( bool x, bool y )
{
    if (x) {
        m_u = m_vertices[U1];
        m_u2 = m_vertices[U3];
        m_vertices[U1] = m_u2;
        m_vertices[U2] = m_u2;
        m_vertices[U3] = m_u;
        m_vertices[U4] = m_u;
    }
    if (y) {
        m_v = m_vertices[V2];
        m_v2 = m_vertices[V1];
        m_vertices[V1] = m_v;
        m_vertices[V2] = m_v2;
        m_vertices[V3] = m_v2;
        m_vertices[V4] = m_v;
    }
}

void Sprite::rotate90( bool clockwise )
{
    if (clockwise) {
        float temp = m_vertices[V1];
        m_vertices[V1] = m_vertices[V4];
        m_vertices[V4] = m_vertices[V3];
        m_vertices[V3] = m_vertices[V2];
        m_vertices[V2] = temp;

        temp = m_vertices[U1];
        m_vertices[U1] = m_vertices[U4];
        m_vertices[U4] = m_vertices[U3];
        m_vertices[U3] = m_vertices[U2];
        m_vertices[U2] = temp;
    } else {
        float temp = m_vertices[V1];
        m_vertices[V1] = m_vertices[V2];
        m_vertices[V2] = m_vertices[V3];
        m_vertices[V3] = m_vertices[V4];
        m_vertices[V4] = temp;

        temp = m_vertices[U1];
        m_vertices[U1] = m_vertices[U2];
        m_vertices[U2] = m_vertices[U3];
        m_vertices[U3] = m_vertices[U4];
        m_vertices[U4] = temp;
    }
}

void Sprite::translate( float xAmount, float yAmount )
{
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
    m_originX = originX;
    m_originY = originY;
    m_dirty = true;
}

void Sprite::setRotation( float degrees )
{
    m_rotation = degrees;
    m_dirty = true;
}

void Sprite::rotate( float degrees )
{
    m_rotation += degrees;
    m_dirty = true;
}

void Sprite::setScale( float scaleXY )
{
    m_scaleX = scaleXY;
    m_scaleY = scaleXY;
    m_dirty = true;
}

void Sprite::setScale( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;
    m_dirty = true;
}

void Sprite::scale( float amount )
{
    m_scaleX += amount;
    m_scaleY += amount;
    m_dirty = true;
}

void Sprite::draw()
{

}

void Sprite::setPostion( float x, float y )
{
    translate(x - m_x - m_originX, y - m_y -m_originY);
}

float Sprite::getX()
{
    return m_x + m_originX;
}

float Sprite::getY()
{
    return m_y + m_originY;
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