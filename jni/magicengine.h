#ifndef _glengine_h_
#define _glengine_h_

#include <GLES2/gl2.h>

class MagicEngine{
	GLuint gProgram;
	GLuint gvPositionHandle;
	GLuint maPositionHandle;
	GLuint maTextureHandle;
	GLuint muMVPMatrixHandle;

public:
	MagicEngine();
	~MagicEngine();

	bool setupGraphics(int w, int h) ;
	void renderFrame();
};


#endif // _glengine_h_