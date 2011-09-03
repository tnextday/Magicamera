#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "glutils.h"


Mesh::Mesh(int width, int height){
    init(width, height);
}

Mesh::Mesh()
{
    mIndexBuffer = NULL;
    mVertexBuffer = NULL;
    mTexCoordBuffer = NULL;
}
Mesh::~Mesh()
{
    glDeleteBuffers(VBO_Ids_Num, m_vboIds);
    SafeDeleteArray(mIndexBuffer);
    SafeDeleteArray(mVertexBuffer);
    SafeDeleteArray(mTexCoordBuffer);
}

void Mesh::setVertexBuffer( GLfloat* vb, int size, int offset /*= 0*/ )
{
    memcpy(mVertexBuffer+offset, vb, size);
    uploadBuffer(BT_VertexBuffer);
}

void Mesh::setTexCoordBuffer( GLfloat* cb, int size, int offset /*= 0*/ )
{
    memcpy(mTexCoordBuffer+offset, cb, size);
    uploadBuffer(BT_TexCoordBuffer);
}

void Mesh::setIndexBuffer( GLushort* ib, int size, int offset /*= 0*/ )
{
    //TODO 有空写
}

void Mesh::set( int i, int j, float x, float y, float u, float v )
{
    if(i>=mW || j>=mH || i< 0 || j < 0)
        LOGE("Mesh::set error");
    setVertex(i,j,x,y);
    setTexCoord(i,j,u,v);
}

void Mesh::setTexCoord( int i, int j, float u, float v )
{
    int index = (mW * j + i)*TexCoordSize;
    mTexCoordBuffer[index++] = u;
    mTexCoordBuffer[index] = v;
}

void Mesh::setVertex( int i, int j, float x, float y )
{
    int index = (mW * j + i)*VertexSize;
    mVertexBuffer[index++] = x;
    mVertexBuffer[index++] = y;
}

bool Mesh::uploadBuffer( BufferType bt /*= BT_VertexBuffer*/ )
{
    switch(bt){
    case BT_IndexBuffer:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[VBO_Element_Idx]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (mW-1)*(mH-1)*6*sizeof(GLushort), mIndexBuffer, GL_STATIC_DRAW);
        break;
    case BT_VertexBuffer:
        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[VBO_Vertex_Idx]);
        glBufferData(GL_ARRAY_BUFFER, mW*mH*VertexSize*sizeof(GLfloat), mVertexBuffer, GL_DYNAMIC_DRAW);
        break;
    case BT_TexCoordBuffer:
        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[VBO_TexCoord_Idx]);
        glBufferData(GL_ARRAY_BUFFER, mW*mH*TexCoordSize*sizeof(GLfloat), mTexCoordBuffer, GL_STATIC_DRAW);
        break;
    }
    return true;
}

bool Mesh::createBufferObjects()
{
    if (m_bGenBuffers) return true;
    m_bGenBuffers = true;
    glGenBuffers(VBO_Ids_Num, m_vboIds);

    
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[VBO_Vertex_Idx]);
    glBufferData(GL_ARRAY_BUFFER, mW*mH*VertexSize*sizeof(GLfloat), mVertexBuffer, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[VBO_TexCoord_Idx]);
    glBufferData(GL_ARRAY_BUFFER, mW*mH*TexCoordSize*sizeof(GLfloat), mTexCoordBuffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[VBO_Element_Idx]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (mW-1)*(mH-1)*6*sizeof(GLushort), mIndexBuffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

void Mesh::draw(BaseShader *shader)
{
    if(!m_bGenBuffers){
        LOGE("You must createBufferObjects before draw");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[VBO_Vertex_Idx]);
    glVertexAttribPointer(shader->getPositionLoc(), VertexSize, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[VBO_TexCoord_Idx]);
    glVertexAttribPointer(shader->getTextureCoordLoc(), TexCoordSize, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[VBO_Element_Idx]);
    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, NULL);
    //glDrawElements(GL_LINES, mIndexCount, GL_UNSIGNED_SHORT, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::init( int width, int height )
{
    //if (mW == width && mH == height) return;
    m_bGenBuffers = false;
    mW = width;
    mH = height;
    int size =  mW * mH;
    int quadW = mW - 1;
    int quadH = mH - 1;
    int quadCount = quadW * quadH;
    mIndexCount = quadCount * 6;
    SafeDeleteArray(mIndexBuffer);
    SafeDeleteArray(mVertexBuffer);
    SafeDeleteArray(mTexCoordBuffer);
    mIndexBuffer = new GLushort[mIndexCount];
    mVertexBuffer = new GLfloat[size*VertexSize];
    mTexCoordBuffer = new GLfloat[size*TexCoordSize];

    /*
    * Initialize triangle list mesh.
    *
    *     [0]-----[  1] ...
    *      |    /   |
    *      |   /    |
    *      |  /     |
    *     [w]-----[w+1] ...
    *      |       |
    *
    */

    int i = 0;
    for (int y = 0; y < quadH; y++) {
        for (int x = 0; x < quadW; x++) {
            GLushort a = (GLushort) (y * mW + x);
            GLushort b = (GLushort) (y * mW + x + 1);
            GLushort c = (GLushort) ((y + 1) * mW + x);
            GLushort d = (GLushort) ((y + 1) * mW + x + 1);

            mIndexBuffer[i++] = a;
            mIndexBuffer[i++] = c;
            mIndexBuffer[i++] = b;

            mIndexBuffer[i++] = b;
            mIndexBuffer[i++] = c;
            mIndexBuffer[i++] = d;
        }
    }
}