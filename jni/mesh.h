#pragma once

#include <GLES2/gl2.h>
#include "baseshader.h"

enum BufferType {
    BT_VertexBuffer = 0,
    BT_TexCoordBuffer,
    BT_IndexBuffer
};


const GLuint VBO_Element_Idx = 0;
const GLuint VBO_TexCoord_Idx = 1;
const GLuint VBO_Vertex_Idx = 2;
const int VBO_Ids_Num = 3;

const int VertexSize = 2;    //only x, y
const int TexCoordSize = 2;


class Mesh
{
protected:
    GLfloat*    mVertexBuffer;    
    GLfloat*    mTexCoordBuffer;
    GLushort*    mIndexBuffer;

    GLuint        m_vboIds[VBO_Ids_Num];

    int            mW;
    int            mH;
    int            mIndexCount;
    bool        m_bGenBuffers;
    
public:
    //widtd与height分别为Mesh上横竖的点数，数值应设定为块数+1
    //如果把一个mesh分成10x20的块，那么width应设置为11，height=21
    Mesh(int width, int height);
    ~Mesh();

    void setVertexBuffer(GLfloat* vb, int size, int offset = 0);
    void setTexCoordBuffer(GLfloat* cb, int size, int offset = 0);
    void setIndexBuffer(GLushort* ib, int size, int offset = 0);
    void set(int i, int j, float x, float y,
            float u, float v);
    void setTexCoord(int i, int j, float u, float v);
    void setVertex(int i, int j, float x, float y);
    bool uploadBuffer(BufferType bt = BT_VertexBuffer);

    bool createBufferObjects();
    void draw(BaseShader *shader);

    //要遍历所有点
    //for(i=0;i<MW();i++){}
    int MW() const { return mW; }
    int MH() const { return mH; }
    int IndexCount() const { return mIndexCount; }

    GLuint getElementBufferObjectId() const { return m_vboIds[VBO_Element_Idx]; }
    GLuint getTexCoordBufferId() const { return m_vboIds[VBO_TexCoord_Idx]; }
    GLuint getVertexBufferObjectId() const { return m_vboIds[VBO_Vertex_Idx]; }
    GLushort* IndexBuffer() const { return mIndexBuffer; }
    GLfloat* TexCoordBuffer() const { return mTexCoordBuffer; }
    GLfloat* VertexBuffer() const { return mVertexBuffer; }

};
