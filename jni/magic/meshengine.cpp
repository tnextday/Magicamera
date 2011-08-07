#include "meshengine.h"
#include "utils/easing.h"
#include "ratetables.h"
#include "glutils/glutils.h"
#include <string.h>
#include <math.h>

MeshEngine::MeshEngine()
{
    m_DestVertex = NULL;
    m_DeltaVertex = NULL;
    m_OrgiVertex = NULL;
    m_bAnimating = false;
    m_bMeshChanged = false;
}
MeshEngine::~MeshEngine()
{
    SafeDeleteArray(m_DestVertex);
    SafeDeleteArray(m_DeltaVertex);
    SafeDeleteArray(m_OrgiVertex);
}

void MeshEngine::update( GLfloat delta )
{
    if (!m_bAnimating) return;
    m_Elapsed += delta;
    
    if (m_Elapsed < m_Duration){
        float percent = m_Elapsed/m_Duration;
        float p = easeShake(percent);
        //LOGI("MeshEngine::update: percent:%.9f;p:%.9f \n", percent, p);
        for (int i = 0; i < m_BufferCount; i++){
            if (m_DeltaVertex[i] != 0.0f)
                mVertexBuffer[i] = m_DestVertex[i] - m_DeltaVertex[i]*p;
        }
    }else{
        m_bAnimating = false;
        memcpy(mVertexBuffer, m_DestVertex, m_BufferCount*sizeof(GLfloat));
    }
    m_bMeshChanged = true;
}

void MeshEngine::backupOrigVertex()
{
    memcpy(m_OrgiVertex, mVertexBuffer, m_BufferCount*sizeof(GLfloat));
}


void MeshEngine::startAnimating(float duration)
{
    for (int i = 0; i < m_BufferCount; i++){
        float delta = m_DestVertex[i] - mVertexBuffer[i];
        if (fabs(delta) >= 1.0f){
            m_DeltaVertex[i] = delta;
        }else 
            m_DeltaVertex[i] = 0.0f;
    }
    m_bAnimating = true;
    m_Duration = duration;
    m_Elapsed = 0.0f;
}

void MeshEngine::moveMesh( float ox, float oy, float mx, float my, float r )
{
    //TODO �߽���
    if(mx == 0 && my == 0) return;
    Vertex *vertexs = (Vertex *)mVertexBuffer;
    Vertex *p;    //point ��ǰ������
    //���������ı�֮������е�
    for (int j = 0; j < mH; j++){
        int baseidx = j*mW;
        for(int i = 0; i < mW; i++){
            int idx = baseidx + i;
            p = vertexs+idx;
            //�ж��Ƿ���Ŀ�귶Χ��
            float dx,dy; //��ǰ���Բ�ĵľ����
            dx = fabs(p->x - ox);
            dy = fabs(p->y - oy);
            if (dx <= r && dy <= r){
                float rate = RateTables[(int)(dy*(RateTablesCount-1)/r)][(int)(dx*(RateTablesCount-1)/r )];
                //float dr = sqrtf(dx*dx + dy*dy);
                if (rate != 0){
                //if (dr <= r){
                    //float rate = (1.0f - dr/r)*0.8;
                    if (i != 0 && i != mW-1)
                        p->x += mx*rate;
                    if (j != 0 && j != mH-1)
                        p->y += my*rate;
                    m_bMeshChanged = true;
                }
            }
        }
    }

}

void MeshEngine::restore()
{
    memcpy(m_DestVertex, m_OrgiVertex, m_BufferCount*sizeof(GLfloat));
    startAnimating(2.0f);
}

void MeshEngine::stopAnimating()
{
    m_bAnimating = false;
    m_Duration = 0.0f;
}

void MeshEngine::onDraw(Texture *texutre)
{
    if (m_bMeshChanged){
        uploadBuffer(BT_VertexBuffer);
        m_bMeshChanged = false;
    }
    if (texutre){
        texutre->bind();
    }else{
        m_InTex->bind();
    }
    glDisable(GL_BLEND);
    Mesh::draw(m_shader);
}

bool MeshEngine::onInit()
{
    init(m_width, m_height);
    generateMesh(m_width, m_height);
    return true;
}

void MeshEngine::finish()
{
//    throw std::exception("The method or operation is not implemented.");
}

void MeshEngine::start()
{
//    throw std::exception("The method or operation is not implemented.");
}

bool MeshEngine::isFinished()
{
//    throw std::exception("The method or operation is not implemented.");
    return false;
}

bool MeshEngine::onTouchDown( float x, float y )
{
    stopAnimating();
    m_lastX = x;
    m_lastY = y;
    return true;
}

bool MeshEngine::onTouchDrag( float x, float y )
{
    moveMesh(x, y, x - m_lastX, y - m_lastY, 100);
    m_lastX = x;
    m_lastY = y;
    return true;
}

bool MeshEngine::onTouchUp( float x, float y )
{
    m_lastX = 0;
    m_lastY = 0;
    return true;
}

void MeshEngine::generateMesh( int w, int h )
{
    int uSteps = MESH_HEIGHT;
    int vSteps = MESH_WIDTH;
    init(uSteps+1, vSteps+1);
    GLfloat x, y,u, v;
    for(int j = 0;j <= vSteps; j++){
        y = j*h/vSteps;
        v = 1 - (GLfloat)j/vSteps;
        for(int i = 0; i <= uSteps; i++){
            x = i*w/uSteps;
            u = (GLfloat)i/uSteps;
            set(i,j,x,y,u,v);
        }
    }
    backupOrigVertex();
    createBufferObjects();
}

void MeshEngine::init( int width, int height )
{
    Mesh::init(width, height);
    m_BufferCount = width*height*VertexSize;
    m_DestVertex = new GLfloat[m_BufferCount];
    m_DeltaVertex = new GLfloat[m_BufferCount];
    m_OrgiVertex = new GLfloat[m_BufferCount];
    m_bAnimating = false;
    m_bMeshChanged = false;
}

EngineType MeshEngine::type()
{
    return EngineType_Mesh;
}