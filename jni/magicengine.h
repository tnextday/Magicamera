#ifndef _glengine_h_
#define _glengine_h_

#include <GLES2/gl2.h>
#include "meshengine.h"
#include "texture.h"

const static int MESH_WIDTH = 20;

void decodeYUV420SP(int* rgb, char* yuv420sp, int width, int height);

class MagicEngine{
	GLuint gProgram;
	GLuint m_positionLoc;
	GLuint m_texCoordLoc;
	GLuint m_viewprojLoc;

	Texture m_PreviewTex;
	MeshEngine*	m_Mesh;

	GLfloat m_ViewWidth;
	GLfloat m_ViewHeight;

public:
	MagicEngine();
	~MagicEngine();

	bool setupGraphics(int w, int h) ;
	void renderFrame();

	void updatePreviewTex(char* data);
	void setPreviewDataInfo(int w, int h, int imageFormat = IMAGE_FORMAT_NV21);

	bool onTouchDown(float x, float y);
	bool onTouchDrag(float x, float y);
	bool onTouchUp(float x, float y);
};


#endif // _glengine_h_