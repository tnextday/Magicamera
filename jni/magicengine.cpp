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
    m_glYUVTex = NULL;
    m_PreviewTex = NULL;
    m_fbo = NULL;

}

MagicEngine::~MagicEngine()
{
    SafeDelete(m_Mesh);
    SafeDelete(m_PreviewTex);
    SafeDelete(m_glYUVTex);
    SafeDelete(m_fbo);
}


bool MagicEngine::setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
    
    LOGI("\n");
    LOGI("setupGraphics(%d, %d)\n", w, h);
    m_ScreenWidth = w;
    m_ScreenHeight = h;
    m_ViewWidth = g_ViewWidth;
    m_ViewHeight = g_ViewWidth*h/w;

    m_shader.makeProgram(gVertexShader, gFragmentShader);
    if (!m_shader.isCompiled()){
        LOGE("Could not create program.\n");
        return false;
    }
    
    m_shader.use();

    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    //启用混合操作
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    m_PreviewTex = new Texture();
    m_PreviewTex->init();
    m_fbo = new FramebufferObject(true);
    printGLColorSpaceInfo();

    m_shader.ortho(0, m_ViewWidth, 0, m_ViewHeight, -10, 10);
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

    return true;
}


void MagicEngine::renderFrame( float delta )
{
    update(delta);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0,0, m_ViewWidth, m_ViewHeight);
    glEnableVertexAttribArray(m_shader.getPositionLoc());
    glEnableVertexAttribArray(m_shader.getTextureCoordLoc());
    drawImage();
    checkGlError("renderFrame");
}

void MagicEngine::setPreviewDataInfo( int w, int h, int imageFormat )
{
    m_PreviewTex->setSize(w, h);

    generateMesh(w, h);
}

void MagicEngine::generateMesh( int w, int h )
{
    SafeDelete(m_Mesh);
    int uSteps = MESH_HEIGHT;
    int vSteps = MESH_WIDTH;
    m_Mesh = new MeshEngine(uSteps+1, vSteps+1);
    GLfloat x, y,u, v;
    for(int j = 0;j <= vSteps; j++){
        y = j*h/vSteps;
        v = 1 - (GLfloat)j/vSteps;
        for(int i = 0; i <= uSteps; i++){
            x = i*w/uSteps;
            u = (GLfloat)i/uSteps;
            m_Mesh->set(i,j,x,y,u,v);
        }
    }
    m_Mesh->backupOrigVertex();
    m_Mesh->createBufferObjects();
}

bool MagicEngine::onTouchDown( float x, float y )
{
    //LOGI("onTouchDown: %.1f, %.1f\n", x, y);
    y = m_ViewHeight - y;
    if(x > m_ViewWidth - 50 && y < 50){
        makePicture(480, 640);
    }else{
        m_Mesh->stopAnimating();
    }
    m_lastX = x;
    m_lastY = y;
    return true;
}

bool MagicEngine::onTouchDrag( float x, float y )
{
    //LOGI("onTouchDrag: %.1f, %.1f\n", x, y);
    y = m_ViewHeight - y;
    m_Mesh->moveMesh(x, y, x - m_lastX, y - m_lastY, 150);
    m_lastX = x;
    m_lastY = y;
    return true;
}

bool MagicEngine::onTouchUp( float x, float y )
{
    //LOGI("onTouchUp: %.1f, %.1f\n", x, y);
    y = m_ViewHeight - y;
    m_lastX = 0;
    m_lastY = 0;
    return true;
}


void MagicEngine::makePicture( int w, int h )
{
    w *= 2;
    h *= 2;
    //FBO只支持RGB565...
    //TODO 支持RGB888
    m_fbo->resizeBuffers(w, h);
    m_fbo->bind();
    glViewport(0,0,w, h);
    //TODO 可变的大小
    m_shader.ortho(0, 480, 0, 640, -10, 10);
    printGLColorSpaceInfo();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    checkGlError("makePicture_0");
    glClear(GL_COLOR_BUFFER_BIT);
    checkGlError("makePicture_1");
    drawImage();
    checkGlError("makePicture_2");

    m_fbo->unbind();

    m_shader.ortho(0, m_ViewWidth, 0, m_ViewHeight, -10, 10);
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
}

void MagicEngine::update( float delta )
{
    m_Mesh->update(delta);
    static float rotateSpeed = 50;
    static float scaleSpeed = 1.5;
    m_testSprite.rotate(rotateSpeed*delta);
    static float scale = 1.0;
    
    if (scale < 0.2){
        scaleSpeed = -scaleSpeed;
        scale = 0.2;
    }else if (scale > 1.0){
        scale = 1.0;
        scaleSpeed = -scaleSpeed;
    }
    scale += scaleSpeed*delta;
    m_testSprite.setScale(scale);
}


void MagicEngine::drawImage()
{
    m_shader.use();
    m_PreviewTex->bind();
    glDisable(GL_BLEND);
    m_Mesh->draw(&m_shader);
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    //m_testSprite.draw(&m_shader);
    
}

