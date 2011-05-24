#include "meshengine.h"
#include "easing.h"
#include <string.h>

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
}

void MeshEngine::update( GLfloat delta )
{
	if (!m_bAnimating) return;
	m_Elapsed += delta;
	if (m_Elapsed < m_Duration){
		float percent = m_Elapsed/m_Duration;
		double p = easeShake(percent);
		for (int i = 0; i < m_BufferCount; i++){
			if (m_DeltaVertex[i] != 0.0f)
				mVertexBuffer[i] = m_DestVertex[i] - m_DeltaVertex[i]*p;
		}
	}else{
		m_bAnimating = false;
		memcpy(mVertexBuffer, m_DestVertex, m_BufferCount*sizeof(GLfloat));
	}
	uploadBuffer();
}

void MeshEngine::backupOrigVertex()
{
	memcpy(m_OrgiVertex, mVertexBuffer, m_BufferCount*sizeof(GLfloat));
}


void MeshEngine::startAnimating(float duration)
{
	for (int i = 0; i < m_BufferCount; i++){
		m_DeltaVertex[i] = m_DestVertex[i] - mVertexBuffer[i];
	}
	m_bAnimating = true;
	m_Duration = duration;
	m_Elapsed = 0.0f;
}