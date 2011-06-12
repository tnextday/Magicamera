#ifndef _glyuvtexture_h_
#define _glyuvtexture_h_

#include <GLES2/gl2.h>
#include "glutils.h"
#include "glHelpers.h"
#include "framebufferobject.h"

const int YTexId_idx = 0;
const int UTexId_idx = 1;
const int VTexId_idx = 2;

class glYUVTexture
{
	GLuint m_Program;
	GLuint m_aPositionLoc;
	GLuint m_aTexCoordLoc;

	//GLuint m_uTexHeightLoc;

	GLuint m_width;
	GLuint m_height;

	char* m_YBuffer;
	char* m_UBuffer;
	char* m_VBuffer;

	GLuint m_YUVTexs[3];

	FramebufferObject* m_fbo;

	bool  m_bInited;

public:
	glYUVTexture(void);
	~glYUVTexture(void);

	bool init(int w, int h, GLuint texid);
	void setTargetTexId(GLuint texid);
	void uploadYUVTexImage(char* yuv420sp, int w, int h);
	void copyUVBuffer(char* yuv420sp);

private:
	void setDefaultTexParameter(GLuint texId);

};
#endif // _glyuvtexture_h_
