#include "meshengine.h"
#include "easing.h"
#include "DeviationMap.h"
#include "glutils.h"
#include <string.h>
#include <math.h>

MeshEngine::MeshEngine( int width, int height )
	:Mesh(width, height)
{
	m_BufferCount = width*height*3;
	m_DestVertex = new GLfloat[m_BufferCount];
	m_DeltaVertex = new GLfloat[m_BufferCount];
	m_OrgiVertex = new GLfloat[m_BufferCount];
	m_bAnimating = false;
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
	uploadBuffer(BT_VertexBuffer);
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
	if(mx == 0 && my == 0) return;
	Vertex *vertexs = (Vertex *)mVertexBuffer;
	Vertex *overtexs = (Vertex *)m_OrgiVertex;
	Vertex *p;	//point 当前点坐标
	Vertex *op;	//原始点坐标
	bool	bMeshChanged = false;
	//遍历除了四边之外的所有点
	for (int j = 1; j < mH-1; j++){
		int baseidx = j*mW;
		for(int i = 1; i < mW-1; i++){
			int idx = baseidx + i;
			p = vertexs+idx;
			op = overtexs+idx;
			//判断是否在目标范围内
			float dx,dy; //当前点和圆心的距离差
			dx = fabs(p->x - ox);
			dy = fabs(p->y - oy);
			if (dx <= r && dy <= r){
				float rate = mapDeviationRate[(int)(dy*(nDeviationRate-1)/r)][(int)(dx*(nDeviationRate-1)/r )];
				float dox,doy; //与原始坐标的差值
				dox = p->x - op->x;
				doy = p->y - op->y;
				if(dox*dox + doy*doy <= 
					(p->x+mx*rate - op->x)*(p->x+mx*rate - op->x) + (p->y+my*rate - op->y)*(p->y+my*rate - op->y)){
					rate *= mapDeviationRate[(int)(doy*(nDeviationRate-1)/r)][(int)(dox*(nDeviationRate-1)/r )];
				}
				p->x += mx*rate;
				p->y += my*rate;
				bMeshChanged = true;
			}
		}
	}
	if (bMeshChanged){
		uploadBuffer(BT_VertexBuffer);
	}
}

void MeshEngine::restore()
{
	memcpy(m_DestVertex, m_OrgiVertex, m_BufferCount*sizeof(GLfloat));
	startAnimating(1.5f);
}

void MeshEngine::stopAnimating()
{
	m_bAnimating = false;
	m_Duration = 0.0f;
}