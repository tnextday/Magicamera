#include "glyuvtexture.h"
#include <string.h>

static const char g_YUV_VShader[] = 
		"uniform mat4 uMVPMatrix;\n"
		"attribute vec4 aPosition;\n"
		"attribute vec2 aTextureCoord;\n"
		"varying vec2 vTextureCoord;\n"
		"void main() {\n"
		"	gl_Position = uMVPMatrix * aPosition;\n" 
		"	vTextureCoord = aTextureCoord;\n"
		"}\n";

static const char g_YUV_FShader[] = 
		"precision mediump float;\n"
		"varying vec2 vTextureCoord;\n"
		"uniform sampler2D Ytex;\n"
		"uniform sampler2D Utex;\n"
		"uniform sampler2D Vtex;\n"
		"void main() {\n"
		"	float r,g,b,y,u,v;\n"
		"	y = texture2D(Ytex, vTextureCoord).r;\n"
		"	u = texture2D(Utex, vTextureCoord/2.0).r;\n"
		"	v = texture2D(Vtex, vTextureCoord/2.0).r;\n"
		"	y = 1.1643*(y-0.0625);\n"
		"	u = u-0.5;\n"
		"	v = v-0.5;\n"
		"	r = y+1.5958*v;\n"
		"	g = y-0.39173*u-0.81290*v;\n"
		"	b = y+2.017*u;\n"
		"	gl_FragColor = vec4(r,g,b,1.0);\n"
		"}\n";

static const GLfloat mTriangleVerticesData[] = {
	// X, Y, Z, U, V
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0, 1.0,
	1.0, 0.0, 0.0, 1.0, 0.0
};

glYUVTexture::glYUVTexture(void)
{
	m_fbo = NULL;
	m_YBuffer = NULL;
	m_UBuffer = NULL;
	m_VBuffer = NULL;
	m_bInited = false;
}

glYUVTexture::~glYUVTexture(void)
{
	if(m_bInited){
		glDeleteTextures(3, m_YUVTexs);
		glDeleteProgram(m_Program);
		SafeDeleteArray(m_YBuffer);
		SafeDeleteArray(m_UBuffer);
		SafeDeleteArray(m_VBuffer);
		SafeDelete(m_fbo);
	}

}

bool glYUVTexture::init( int w, int h, GLuint texid )
{
	LOGI("glYUVTexture::init() start\n");
	m_Program = createProgram(g_YUV_VShader, g_YUV_FShader);
	if (!m_Program) {
		LOGE("Could not create program.\n");
		return false;
	}
	glUseProgram(m_Program);

	m_aPositionLoc = glGetAttribLocation(m_Program, "aPosition");
	checkGlError("glGetAttribLocation aPosition");
	if (m_aPositionLoc == -1) {
		LOGE("Could not get attrib location for aPosition");
		return false;
	}

	m_aTexCoordLoc = glGetAttribLocation(m_Program, "aTextureCoord");
	checkGlError("glGetAttribLocation aTextureCoord");
	if (m_aTexCoordLoc == -1) {
		LOGE("Could not get attrib location for aTextureCoord");
		return false;
	}

	GLfloat mvp[16];
	matIdentity(mvp);
	// 设置视口的大小
	matOrtho(mvp, 0, 1.0, 0, 1.0, -10, 10);
	glUniformMatrix4fv(glGetUniformLocation(m_Program, "uMVPMatrix"), 1, GL_FALSE, (GLfloat*)mvp);

	//m_uTexHeightLoc = glGetUniformLocation(m_Program, "texHeight");
	

	glGenTextures(3, m_YUVTexs);

	glUniform1i(glGetUniformLocation(m_Program, "Ytex"), 0);
	checkGlError("glUniform1i_0");
	glUniform1i(glGetUniformLocation(m_Program, "Utex"), 1);
	checkGlError("glUniform1i_1");
	glUniform1i(glGetUniformLocation(m_Program, "Vtex"), 2);
	checkGlError("glUniform1i_2");

	m_width = w;
	m_height = h;
	int size = w*h;
	m_YBuffer = new char[size];
	m_UBuffer = new char[size/4];
	m_VBuffer = new char[size/4];
	
	m_fbo = new FramebufferObject();
	m_fbo->texture2d(texid);
	checkGlError("glYUVTexture::init");
	LOGI("glYUVTexture::init() end\n");
	m_bInited = true;
	return true;
}

void glYUVTexture::setDefaultTexParameter( GLuint texId )
{
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	checkGlError("setDefaultTexParameter");
}

void glYUVTexture::uploadYUVTexImage( char* yuv420sp, int w, int h )
{
	if (w != m_width || h != m_height) {
		LOGE("uploadYUVTexImage Error 0\n");
		return;
	}
	copyYUVBuffer(yuv420sp);
	glUseProgram(m_Program); 
	glDisable(GL_DEPTH_TEST);
	m_fbo->bind();
	glViewport(0, 0, m_width, m_height);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	checkGlError("glYUVTexture::uploadYUVTexImage_4_0");
	glClear(GL_COLOR_BUFFER_BIT);
	checkGlError("glYUVTexture::uploadYUVTexImage_4_1");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_YUVTexs[YTexId_idx]);
	glUniform1i(glGetUniformLocation(m_Program, "Ytex"), 0);
	setDefaultTexParameter(m_YUVTexs[YTexId_idx]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,w,h,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,yuv420sp);

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_YUVTexs[UTexId_idx]);
	glUniform1i(glGetUniformLocation(m_Program, "Utex"), 1);
	setDefaultTexParameter(m_YUVTexs[UTexId_idx]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,w/2,h/2,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,m_UBuffer);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_YUVTexs[VTexId_idx]);
	glUniform1i(glGetUniformLocation(m_Program, "Vtex"), 2);
	setDefaultTexParameter(m_YUVTexs[VTexId_idx]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,w/2,h/2,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,m_VBuffer);
	checkGlError("glYUVTexture::uploadYUVTexImage_8");
	
	glEnableVertexAttribArray(m_aPositionLoc);
	glEnableVertexAttribArray(m_aTexCoordLoc);
	glVertexAttribPointer(m_aPositionLoc, 3, GL_FLOAT, GL_FALSE, 5*4, mTriangleVerticesData);
	glVertexAttribPointer(m_aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 5*4, mTriangleVerticesData);
	checkGlError("glYUVTexture::uploadYUVTexImage_9_0");
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	checkGlError("glYUVTexture::uploadYUVTexImage_9_1");
	m_fbo->unbind();
	checkGlError("glYUVTexture::uploadYUVTexImage");
}

void glYUVTexture::setTargetTexId( GLuint texid )
{
	if (m_bInited)
	{
		m_fbo->texture2d(texid);
	}
}

void glYUVTexture::copyYUVBuffer( char* yuv420sp )
{
	static int size = m_width*m_height;
	//memcpy(m_YBuffer, yuv420sp, size);
	char* tu = m_UBuffer;
	char* tv = m_VBuffer;
	char* uv = yuv420sp + size; 
	for(int i = 0; i < size/4;i++){
		*(tv++) = *(uv++);
		*(tu++) = *(uv++);
	}
}