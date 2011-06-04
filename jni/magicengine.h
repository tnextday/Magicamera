#ifndef _glengine_h_
#define _glengine_h_

#include <GLES2/gl2.h>
#include "meshengine.h"
#include "texture.h"

const static int MESH_WIDTH = 20;

const int IMAGE_FORMAT_RGB_565	= 0x00000004; //4
const int IMAGE_FORMAT_NV21		= 0x00000011; //17
const int IMAGE_FORMAT_PACKET	= 0x00000100; //256  ���ѹ�������ݣ�jpeg,png,tga,bitmap...

void decodeYUV420SP(char* rgb565, char* yuv420sp, int width, int height);

class MagicEngine{
	GLuint m_Program;
	GLuint m_positionLoc;
	GLuint m_texCoordLoc;
	GLuint m_viewprojLoc;

	Texture* m_PreviewTex;
	MeshEngine*	m_Mesh;

	GLfloat m_ViewWidth;
	GLfloat m_ViewHeight;
	
	int		m_inputFortmat;
	char*	m_tmpImageData;

public:
	MagicEngine();
	~MagicEngine();

	bool setupGraphics(int w, int h) ;
	void renderFrame();

	void updatePreviewTex(char* data, long len);
	void setPreviewDataInfo(int w, int h, int imageFormat = IMAGE_FORMAT_NV21);

	bool onTouchDown(float x, float y);
	bool onTouchDrag(float x, float y);
	bool onTouchUp(float x, float y);
	void drawTexture( Texture *tex, GLint posX, GLint posY );
};


#endif // _glengine_h_