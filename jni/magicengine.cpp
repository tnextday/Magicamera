

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "magicengine.h"
#include "glutils.h"


static const char gVertexShader[] = 
        "uniform mat4 uMVPMatrix;\n"
        "attribute vec4 aPosition;\n"
        "attribute vec2 aTextureCoord;\n"
        "varying vec2 vTextureCoord;\n"
        "void main() {\n"
        "  gl_Position = uMVPMatrix * aPosition;\n" 
        "  vTextureCoord = aTextureCoord;\n"
        "}\n";

static const char gFragmentShader[] = 
		"precision mediump float;\n"
		"varying vec2 vTextureCoord;\n"
		"uniform sampler2D sTexture;\n"
		"void main() {\n"
		"  gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
		"}\n";




MagicEngine::MagicEngine()
{
	m_Mesh = NULL;
}

MagicEngine::~MagicEngine()
{
	if (m_Mesh){
		delete m_Mesh;
		m_Mesh = NULL;
	}
}


bool MagicEngine::setupGraphics(int w, int h) {
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

	LOGI("setupGraphics(%d, %d)", w, h);
	gProgram = createProgram(gVertexShader, gFragmentShader);
	if (!gProgram) {
		LOGE("Could not create program.");
		return false;
	}

	maPositionHandle = glGetAttribLocation(gProgram, "aPosition");
	checkGlError("glGetAttribLocation aPosition");
	if (maPositionHandle == -1) {
		LOGE("Could not get attrib location for aPosition");
		return false;
	}
	maTextureHandle = glGetAttribLocation(gProgram, "aTextureCoord");
	checkGlError("glGetAttribLocation aTextureCoord");
	if (maTextureHandle == -1) {
		LOGE("Could not get attrib location for aTextureCoord");
		return false;
	}

	muMVPMatrixHandle = glGetUniformLocation(gProgram, "uMVPMatrix");
	checkGlError("glGetUniformLocation uMVPMatrix");
	if (muMVPMatrixHandle == -1) {
		LOGE("Could not get attrib location for uMVPMatrix");
		return false;
	}

	m_ViewWidth = w;
	m_ViewHeight = h;

	glViewport(0, 0, w, h);
	checkGlError("glViewport");
	return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
0.5f, -0.5f };

void MagicEngine::renderFrame() {
	static float grey;
	grey += 0.01f;
	if (grey > 1.0f) {
		grey = 0.0f;
	}
	glClearColor(grey, grey, grey, 1.0f);
	checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	glUseProgram(gProgram);
	checkGlError("glUseProgram");

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(gvPositionHandle);
	checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	checkGlError("glDrawArrays");
}

void MagicEngine::updatePreviewTex( char* data)
{
	m_PreviewTex.uploadImageData((GLubyte*)data);
}

void MagicEngine::setPreviewInfo( int w, int h, int imageFormat /*= GL_RGB565*/ )
{
	m_PreviewTex.setSize(w, h);
	m_PreviewTex.setImageFormat(imageFormat);
	m_Mesh = new Mesh();
}