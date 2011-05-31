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
	//坐标差值
	GLfloat*	m_DeltaVertex;
	//目标坐标
	GLfloat*	m_DestVertex;
	//原始坐标，恢复用
	GLfloat*	m_OrgiVertex;
	int			m_BufferCount;
	
	bool		m_bAnimating;
	float		m_Duration;
	float		m_Elapsed;	//逝去时间


public:
	MeshEngine(int width, int height);
	~MeshEngine();

	void update(GLfloat delta);

	//设置完坐标后，调用此函数备份原始坐标
	void backupOrigVertex();

	//set destvertex then start animating
	//Animated duration in sec
	void startAnimating(float duration);

};
#endif // _meshengine_h_