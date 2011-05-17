#ifndef _glengine_h_
#define _glengine_h_

#include <GLES2/gl2.h>

class GLEngine{
	GLuint gProgram;
	GLuint gvPositionHandle;
	GLuint maPositionHandle;
	GLuint maTextureHandle;
	GLuint muMVPMatrixHandle;

public:
	GLEngine();
	~GLEngine();

	bool setupGraphics(int w, int h) ;
	void renderFrame();
};


#endif // _glengine_h_