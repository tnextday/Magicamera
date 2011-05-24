

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "magicengine.h"
#include "glutils.h"
#include "glHelpers.h"



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
	SafeDelete(m_Mesh);
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

	m_positionLoc = glGetAttribLocation(gProgram, "aPosition");
	checkGlError("glGetAttribLocation aPosition");
	if (m_positionLoc == -1) {
		LOGE("Could not get attrib location for aPosition");
		return false;
	}
	m_texCoordLoc = glGetAttribLocation(gProgram, "aTextureCoord");
	checkGlError("glGetAttribLocation aTextureCoord");
	if (m_texCoordLoc == -1) {
		LOGE("Could not get attrib location for aTextureCoord");
		return false;
	}

	m_viewprojLoc = glGetUniformLocation(gProgram, "uMVPMatrix");
	checkGlError("glGetUniformLocation uMVPMatrix");
	if (m_viewprojLoc == -1) {
		LOGE("Could not get attrib location for uMVPMatrix");
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	//glBlendColor(0.0, 0.0, 0.0, 0.0);
	//启用混合操作
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	m_ViewWidth = w;
	m_ViewHeight = h;

	glViewport(0, 0, w, h);
	checkGlError("glViewport");

	//使用2D投影,笛卡尔坐标系，宽高为屏幕宽高
	GLfloat mvp[16];
	matIdentity(mvp);
	matOrtho(mvp, 0, w, 0, h, -1, 1);
	glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (float*)mvp);
	return true;
}



void MagicEngine::renderFrame() {

	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	glUseProgram(gProgram);
	checkGlError("glUseProgram");

	
	m_PreviewTex.bind();
	m_Mesh->draw();
}

void MagicEngine::updatePreviewTex( char* data )
{
	m_PreviewTex.uploadImageData((GLubyte*)data);
}

void MagicEngine::setPreviewInfo( int w, int h, int imageFormat /*= GL_RGB565*/ )
{
	m_PreviewTex.setSize(w, h);
	m_PreviewTex.setImageFormat(imageFormat);
	if (m_Mesh){
		SafeDelete(m_Mesh);
	}
	int mw = MESH_WIDTH+1;
	int mh = MESH_WIDTH*w/h + 1;
	m_Mesh = new MeshEngine(mw, mh);
	GLfloat x = 0, y = 0;
	GLfloat xStep = m_ViewWidth/(mw-1);
	GLfloat yStep = m_ViewHeight/(mh-1);
	for(int j = 0;j < mh; j++){
		x = 0;
		for(int i = 0; i < mw; i++){
			m_Mesh->setVertex(i, j, x, y, 0);
			x += xStep;
		}
		y += yStep;
	}
	m_Mesh->backupOrigVertex();
}

bool MagicEngine::onTouchDown( float x, float y )
{
	y = m_ViewHeight - y;
	//TODO onTouchDown
	return true;
}

bool MagicEngine::onTouchDrag( float x, float y )
{
	y = m_ViewHeight - y;
	//TODO onTouchDrag
	return true;
}

bool MagicEngine::onTouchUp( float x, float y )
{
	y = m_ViewHeight - y;
	//TODO onTouchUp
	return true;
}