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
    m_InTex = NULL;
    m_OutTex = NULL;
    m_shader = NULL;
    m_fbo = NULL;
}

MagicEngine::MagicEngine( BaseShader* shader, Texture* srcTex )
{
    m_Mesh = NULL;
    init(shader, srcTex);
}

MagicEngine::~MagicEngine()
{
    SafeDelete(m_Mesh);
    SafeDelete(m_fbo);
    SafeDelete(m_OutTex);
}


bool MagicEngine::init(BaseShader* shader, Texture* srcTex) {
    m_OutTex = new Texture();
    m_OutTex->init();

    setInTexture(srcTex);
    setShader(shader);
    //默认纹理大小设置成480x640
    setSize(g_CoordWidth, g_CoordHeight);

    m_fbo = new FramebufferObject();
    m_fbo->texture2d(m_OutTex->getTexHandle());

    //内建坐标系为480x640大小的坐标，此值为固定值
    matIdentity(m_vp);
    matOrtho(m_vp, 0, g_CoordWidth, 0, g_CoordHeight, -10, 10);
    generateMesh(g_CoordWidth, g_CoordHeight);
    //TODO remove test
    m_testSprite.loadTexture("f:\\btn_04.png");
    m_testSprite.setPostion(240, 320);
    return true;
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
    m_Mesh->stopAnimating();
    m_lastX = x;
    m_lastY = y;
    return true;
}

bool MagicEngine::onTouchDrag( float x, float y )
{
    m_Mesh->moveMesh(x, y, x - m_lastX, y - m_lastY, 150);
    m_lastX = x;
    m_lastY = y;
    return true;
}

bool MagicEngine::onTouchUp( float x, float y )
{
    m_lastX = 0;
    m_lastY = 0;
    return true;
}

void MagicEngine::update( float delta )
{
    m_Mesh->update(delta);
}


void MagicEngine::drawImage()
{
    m_fbo->bind();
    glViewport(0,0, m_width, m_height);
    draw();
    m_fbo->unbind();
}

//设置图片大小，不同于坐标
void MagicEngine::setSize( int w, int h )
{
    m_width = w;
    m_height = h;
    if (m_OutTex){
        m_OutTex->setSize(w, h);
    }
}

void MagicEngine::restore()
{
    m_Mesh->restore();
}

void MagicEngine::SaveImage( int w, int h )
{
    int ow = m_width, oh = m_height;
    setSize(w, h);

    m_fbo->bind();
    draw();

    GLubyte* pixels = new GLubyte[w*h*4];

    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    if(m_saveImage){
        m_saveImage->SaveImage((char*)pixels, w, h, 0);
    }
    delete[] pixels;
    checkGlError("SavePicture");
    m_fbo->unbind();

    setSize(ow, oh);
}

void MagicEngine::draw()
{
    m_shader->use();
    m_shader->setViewProj(m_vp);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(m_shader->getPositionLoc());
    glEnableVertexAttribArray(m_shader->getTextureCoordLoc());
    m_InTex->bind();
    glDisable(GL_BLEND);
    m_Mesh->draw(m_shader);
    //TODO remove test
    m_testSprite.draw(m_shader);
}

