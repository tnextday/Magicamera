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
    m_easing = NULL;
}
MeshEngine::~MeshEngine()
{
    SafeDeleteArray(m_DestVertex);
    SafeDeleteArray(m_DeltaVertex);
    SafeDeleteArray(m_OrgiVertex);
    SafeDelete(m_easing);
}

void MeshEngine::update( GLfloat delta )
{
    if (!m_bAnimating) return;
    m_Elapsed += delta;
    if (m_Elapsed < m_Duration){
        float percent = m_Elapsed/m_Duration;
        if (m_easing)
            percent = 1 - m_easing->update(percent);
        for (int i = 0; i < m_BufferCount; i++){
            if (m_DeltaVertex[i] != 0.0f)
                mVertexBuffer[i] = m_DestVertex[i] - m_DeltaVertex[i]*percent;
        }
    }else{
        if (m_toFinish)
            m_finished = true;
        m_bAnimating = false;
        memcpy(mVertexBuffer, m_DestVertex, m_BufferCount*sizeof(GLfloat));
    }
    m_bMeshChanged = true;
}

void MeshEngine::backupOrigVertex()
{
    memcpy(m_OrgiVertex, mVertexBuffer, m_BufferCount*sizeof(GLfloat));
}


void MeshEngine::startAnimating(float duration, CEasing *easing /*= NULL*/)
{
    if (easing){
        SafeDelete(m_easing);
        m_easing = easing;
    }
    
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
    //TODO 边界检查
    if(mx == 0 && my == 0) return;
    Vertex *vertexs = (Vertex *)mVertexBuffer;
    Vertex *p;    //point 当前点坐标
    //遍历除了四边之外的所有点
    for (int j = 0; j < mH; j++){
        int baseidx = j*mW;
        for(int i = 0; i < mW; i++){
            int idx = baseidx + i;
            p = vertexs+idx;
            //判断是否在目标范围内
            float dx,dy; //当前点和圆心的距离差
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
    startAnimating(2.0f, new CEaseShake());
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
//    create(m_width, m_height);
    return true;
}

void MeshEngine::finish()
{
    m_toFinish = true;
    memcpy(m_DestVertex, m_OrgiVertex, m_BufferCount*sizeof(GLfloat));
    startAnimating(0.2f, new CEaseOutCubic());
}

void MeshEngine::start()
{
    m_toFinish = false;
    m_finished = false;
}

bool MeshEngine::isFinished()
{
    return m_finished;
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
    Mesh::init(uSteps+1, vSteps+1);
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
    createBufferObjects();
}

void MeshEngine::init( int width, int height )
{
    if (mW == width && mH == height) return;
    generateMesh(width, height);
    m_BufferCount = mW*mH*VertexSize;
    SafeDeleteArray(m_DestVertex);
    SafeDeleteArray(m_DeltaVertex);
    SafeDeleteArray(m_OrgiVertex);
    m_DestVertex = new GLfloat[m_BufferCount];
    m_DeltaVertex = new GLfloat[m_BufferCount];
    m_OrgiVertex = new GLfloat[m_BufferCount];
    m_bAnimating = false;
    m_bMeshChanged = false;
    backupOrigVertex();
}

EngineType MeshEngine::type()
{
    return EngineType_Mesh;
}

void MeshEngine::resizeCoord()
{
    MagicEngine::resizeCoord();
    init(m_width, m_height);
}