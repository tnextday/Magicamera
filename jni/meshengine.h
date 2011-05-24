#ifndef _meshengine_h_
#define _meshengine_h_
#include "mesh.h"
#include <GLES2/gl2.h>

struct Vertex {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class MeshEngine : public Mesh{

private:
	//�����ֵ
	GLfloat*	m_DeltaVertex;
	//Ŀ������
	GLfloat*	m_DestVertex;
	//ԭʼ���꣬�ָ���
	GLfloat*	m_OrgiVertex;
	int			m_BufferCount;
	
	bool		m_bAnimating;
	float		m_Duration;
	float		m_Elapsed;

public:
	MeshEngine(int width, int height);
	~MeshEngine();

	void update(GLfloat delta);

	//����������󣬵��ô˺�������ԭʼ����
	void backupOrigVertex();

	//set destvertex then start animating
	//Animated duration in sec
	void startAnimating(float duration);

};
#endif // _meshengine_h_