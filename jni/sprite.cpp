#include "sprite.h"

Sprite::Sprite()
{
    scaleX = 0;
    scaleY = 0;
    dirty = true;
}

float* Sprite::getVertices()
{
    if (dirty) {
        dirty = false;

        float localX = -originX;
        float localY = -originY;
        float localX2 = localX + width;
        float localY2 = localY + height;
        float worldOriginX = x - localX;
        float worldOriginY = y - localY;
        if (scaleX != 1 || scaleY != 1) {
            localX *= scaleX;
            localY *= scaleY;
            localX2 *= scaleX;
            localY2 *= scaleY;
        }
        if (rotation != 0) {
            float cos,sin;
//             cos = MathUtils.cosDeg(rotation);
//             sin = MathUtils.sinDeg(rotation);
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
            vertices[X1] = x1;
            vertices[Y1] = y1;

            float x2 = localXCos - localY2Sin + worldOriginX;
            float y2 = localY2Cos + localXSin + worldOriginY;
            vertices[X2] = x2;
            vertices[Y2] = y2;

            float x3 = localX2Cos - localY2Sin + worldOriginX;
            float y3 = localY2Cos + localX2Sin + worldOriginY;
            vertices[X3] = x3;
            vertices[Y3] = y3;

            vertices[X4] = x1 + (x3 - x2);
            vertices[Y4] = y3 - (y2 - y1);
        } else {
            float x1 = localX + worldOriginX;
            float y1 = localY + worldOriginY;
            float x2 = localX2 + worldOriginX;
            float y2 = localY2 + worldOriginY;

            vertices[X1] = x1;
            vertices[Y1] = y1;

            vertices[X2] = x1;
            vertices[Y2] = y2;

            vertices[X3] = x2;
            vertices[Y3] = y2;

            vertices[X4] = x2;
            vertices[Y4] = y1;
        }
    }
    return vertices;
}

void Sprite::flip( bool x, bool y )
{
    float temp;
    if (x) {
        u = vertices[U1];
        u2 = vertices[U3];
        vertices[U1] = u2;
        vertices[U2] = u2;
        vertices[U3] = u;
        vertices[U4] = u;
    }
    if (y) {
        v = vertices[V2];
        v2 = vertices[V1];
        vertices[V1] = v;
        vertices[V2] = v2;
        vertices[V3] = v2;
        vertices[V4] = v;
    }
}

void Sprite::rotate90( bool clockwise )
{
    if (clockwise) {
        float temp = vertices[V1];
        vertices[V1] = vertices[V4];
        vertices[V4] = vertices[V3];
        vertices[V3] = vertices[V2];
        vertices[V2] = temp;

        temp = vertices[U1];
        vertices[U1] = vertices[U4];
        vertices[U4] = vertices[U3];
        vertices[U3] = vertices[U2];
        vertices[U2] = temp;
    } else {
        float temp = vertices[V1];
        vertices[V1] = vertices[V2];
        vertices[V2] = vertices[V3];
        vertices[V3] = vertices[V4];
        vertices[V4] = temp;

        temp = vertices[U1];
        vertices[U1] = vertices[U2];
        vertices[U2] = vertices[U3];
        vertices[U3] = vertices[U4];
        vertices[U4] = temp;
    }
}