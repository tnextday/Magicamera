#ifndef _meshengine_h_
#define _meshengine_h_
#include "mesh.h"
#include <GLES2/gl2.h>


class MeshEngine : public Mesh{

public:
	void update(GLfloat delta);

};
#endif // _meshengine_h_