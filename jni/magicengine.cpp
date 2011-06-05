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
	m_tmpImageData = NULL;
}

MagicEngine::~MagicEngine()
{
	SafeDeleteArray(m_tmpImageData);
	SafeDelete(m_Mesh);
	SafeDelete(m_PreviewTex);
}


bool MagicEngine::setupGraphics(int w, int h) {
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	
	LOGI("\n");
	LOGI("setupGraphics(%d, %d)\n", w, h);
	m_Program = createProgram(gVertexShader, gFragmentShader);
	if (!m_Program) {
		LOGE("Could not create program.\n");
		return false;
	}
	glUseProgram(m_Program);

	m_positionLoc = glGetAttribLocation(m_Program, "aPosition");
	checkGlError("glGetAttribLocation aPosition");
	if (m_positionLoc == -1) {
		LOGE("Could not get attrib location for aPosition");
		return false;
	}
	m_texCoordLoc = glGetAttribLocation(m_Program, "aTextureCoord");
	checkGlError("glGetAttribLocation aTextureCoord");
	if (m_texCoordLoc == -1) {
		LOGE("Could not get attrib location for aTextureCoord");
		return false;
	}

	m_viewprojLoc = glGetUniformLocation(m_Program, "uMVPMatrix");
	checkGlError("glGetUniformLocation uMVPMatrix");
	if (m_viewprojLoc == -1) {
		LOGE("Could not get attrib location for uMVPMatrix");
		return false;
	}

	glDisable(GL_DEPTH_TEST);
// 	glCullFace(GL_BACK);
// 	glEnable(GL_CULL_FACE);
	//启用混合操作
 	glDisable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA	glEnable(GL_TEXTURE_2D);

	m_ViewWidth = w;
	m_ViewHeight = h;

	glViewport(0, 0, w, h);

	//使用2D投影,笛卡尔坐标系，宽高为屏幕宽高
	GLfloat mvp[16];
	matIdentity(mvp);
	// 设置视口的大小
	matOrtho(mvp, 0, w, 0, h, -10, 10);
	//设置镜头
	//  	matLookAt((float*)mat_t, 0,0, -0.5, 0,0,0, 0,1,0);
	//  	matMult((float*)mvp, (float*)mat_p, (float*)mat_t);	
	glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);

	m_PreviewTex = new Texture();
	return true;
}


void MagicEngine::drawTexture( Texture *tex, GLint posX, GLint posY )
{
	static const GLfloat texCoord[] =
	{
		0.0, 0.0,
		0.0, 1.0, 
		1.0, 1.0,
		1.0, 0.0,
	};
	GLfloat texVertex[12] = {0};
	GLint w,h;
	w = tex->m_Width/2;
	h = tex->m_Height/2;

	texVertex[0] = posX-w; texVertex[1] = posY+h; texVertex[2] = 0;
	texVertex[3] = posX-w; texVertex[4] = posY-h; texVertex[5] = 0;
	texVertex[6] = posX+w; texVertex[7] = posY-h; texVertex[8] = 0;
	texVertex[9] = posX+w; texVertex[10] = posY+h; texVertex[11] = 0;
	
	glEnableVertexAttribArray(m_positionLoc);
	glEnableVertexAttribArray(m_texCoordLoc);
	tex->bind();
	glVertexAttribPointer(m_positionLoc, 3, GL_FLOAT, GL_FALSE, 0, texVertex);
	glVertexAttribPointer(m_texCoordLoc, 2, GL_INT, GL_FALSE, 0, texCoord);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void MagicEngine::renderFrame( float delta )
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(m_Program);
	
	//LOGI("renderFrame delta time = %.6f\n", delta);
	m_Mesh->update(delta);
/*	drawTexture(m_PreviewTex, m_ViewWidth/2, m_ViewHeight/2);*/
 	m_PreviewTex->bind();
 	m_Mesh->draw();
	checkGlError("renderFrame");
}

void MagicEngine::updatePreviewTex( char* data, long len )
{
	if (m_inputFortmat == IMAGE_FORMAT_NV21){
		decodeYUV420SP(m_tmpImageData, data, m_PreviewTex->m_Width, m_PreviewTex->m_Height);
		m_PreviewTex->uploadImageData((GLubyte*)m_tmpImageData);
	}else if(m_inputFortmat == IMAGE_FORMAT_RGB_565){
		m_PreviewTex->uploadImageData((GLubyte*)data);
	}else if(m_inputFortmat == IMAGE_FORMAT_PACKET){
		m_PreviewTex->uploadImageData((unsigned char*)data, len);
	}
	
	
}

void MagicEngine::setPreviewDataInfo( int w, int h, int imageFormat )
{
	m_PreviewTex->setSize(w, h);
	m_inputFortmat = imageFormat;

	if (m_inputFortmat == IMAGE_FORMAT_NV21){
		m_PreviewTex->setImageFormat(GDX2D_FORMAT_RGB565);
		m_tmpImageData = new char[w*h*2];
	}if(m_inputFortmat == IMAGE_FORMAT_RGB_565)
		m_PreviewTex->setImageFormat(GDX2D_FORMAT_RGB565);

	generateMesh(w, h);
}

void MagicEngine::generateMesh( int w, int h )
{
	SafeDelete(m_Mesh);
	int uSteps = MESH_HEIGHT*w/h;
	int vSteps = MESH_HEIGHT;
	m_Mesh = new MeshEngine(uSteps+1, vSteps+1);
	m_Mesh->setPositionLoc(m_positionLoc);
	m_Mesh->setTexCoordLoc(m_texCoordLoc);
	GLfloat x, y,u, v;
	for(int j = 0;j <= vSteps; j++){
		y = j*h/vSteps;
		v = 1 - (GLfloat)j/vSteps;
		for(int i = 0; i <= uSteps; i++){
			x = i*w/uSteps;
			u = (GLfloat)i/uSteps;
			m_Mesh->set(i,j,x,y,0,u,v);
		}
	}
	m_Mesh->backupOrigVertex();
	m_Mesh->createBufferObjects();
}

bool MagicEngine::onTouchDown( float x, float y )
{
	LOGI("onTouchDown: %.1f, %.1f\n", x, y);
	y = m_ViewHeight - y;
	if (x > m_ViewWidth - 50 && y > m_ViewHeight -50){
		m_Mesh->restore();
	}
	m_Mesh->stopAnimating();
	m_lastX = x;
	m_lastY = y;
	return true;
}

bool MagicEngine::onTouchDrag( float x, float y )
{
	LOGI("onTouchDrag: %.1f, %.1f\n", x, y);
	y = m_ViewHeight - y;
	m_Mesh->moveMesh(x, y, x - m_lastX, y - m_lastY, 150);
	m_lastX = x;
	m_lastY = y;
	return true;
}

bool MagicEngine::onTouchUp( float x, float y )
{
	LOGI("onTouchUp: %.1f, %.1f\n", x, y);
	//y = m_ViewHeight - y;
	m_lastX = 0;
	m_lastY = 0;
	return true;
}


inline void decodeYUV420SP( char* rgb565, char* yuv420sp, int width, int height )
{
	int frameSize = width * height;
	unsigned short* dest_ptr = (unsigned short*)rgb565;
	//rgb全部左移8位，变成整数计算
	for (int j = 0, yp = 0; j < height; j++) {
		int uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
		for (int i = 0; i < width; i++, yp++) {
			int y = (0xff & ((int) yuv420sp[yp])) - 16;
			if (y < 0)
				y = 0;
			if ((i & 1) == 0) {
				v = (0xff & yuv420sp[uvp++]) - 128;
				u = (0xff & yuv420sp[uvp++]) - 128;
			}

			int y1192 = 1192 * y;
			int r = (y1192 + 1634 * v);
			int g = (y1192 - 833 * v - 400 * u);
			int b = (y1192 + 2066 * u);

			if (r < 0)
				r = 0;
			else if (r > 262143)
				r = 262143;
			if (g < 0)
				g = 0;
			else if (g > 262143)
				g = 262143;
			if (b < 0)
				b = 0;
			else if (b > 262143)
				b = 262143;

			//还原 rgb >> 8 位
			dest_ptr[yp] = (unsigned(r) & 0xF800)|((unsigned(g) >> 5) & 0x7E0) | (unsigned(b) >> 11);
		}
	}
}
