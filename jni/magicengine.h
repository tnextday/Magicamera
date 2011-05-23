#ifndef _glengine_h_
#define _glengine_h_

#include <GLES2/gl2.h>

const static int MESH_WIDTH = 20;

class MagicEngine{
	GLuint gProgram;
	GLuint gvPositionHandle;
	GLuint maPositionHandle;
	GLuint maTextureHandle;
	GLuint muMVPMatrixHandle;

	Texture m_PreviewTex;
	Mesh*	m_Mesh;

	int m_ViewWidth;
	int m_ViewHeight;

public:
	MagicEngine();
	~MagicEngine();

	bool setupGraphics(int w, int h) ;
	void renderFrame();

	void updatePreviewTex(char* data);
	void setPreviewInfo(int w, int h, int imageFormat = GL_RGB565);

	bool onTouchDown(float x, float y);
	bool onTouchDrag(float x, float y);
	bool onTouchUp(float x, float y);
};


#endif // _glengine_h_