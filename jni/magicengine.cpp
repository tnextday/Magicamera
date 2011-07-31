#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "magicengine.h"
#include "glutils.h"
#include "glHelpers.h"

MagicEngine::MagicEngine()
{
    m_Mesh = NULL;
    m_InTex = NULL;
    m_shader = NULL;
    m_fbo = NULL;
    m_sfactor = GL_SRC_ALPHA;
    m_dfactor = GL_ONE_MINUS_SRC_ALPHA;
    m_saveImage = NULL;
    m_sizeChange = NULL;
}

MagicEngine::MagicEngine( BaseShader* shader, Texture* srcTex )
{
    m_Mesh = NULL;
    m_sfactor = GL_SRC_ALPHA;
    m_dfactor = GL_ONE_MINUS_SRC_ALPHA;
    m_saveImage = NULL;
    m_sizeChange = NULL;
    init(shader, srcTex);
}

MagicEngine::~MagicEngine()
{
    SafeDelete(m_Mesh);
    SafeDelete(m_fbo);
}


bool MagicEngine::init(BaseShader* shader, Texture* srcTex) {
    m_OutTex.init();
    m_coverTex.init();

    setInputTexture(srcTex);
    setShader(shader);

    m_coordWidth = g_CoordWidth;
    m_coordHeight = m_coordWidth*srcTex->getHeight()/srcTex->getWidth();
    setSize(m_coordWidth, m_coordHeight);

    m_fbo = new FramebufferObject();
    m_fbo->texture2d(m_OutTex.getTexHandle());

    //内建坐标系为480x640大小的坐标，此值为固定值
    matIdentity(m_vp);
    matOrtho(m_vp, 0, m_coordWidth, 0, m_coordHeight, -10, 10);
    generateMesh(m_coordWidth, m_coordHeight);
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
    m_Mesh->moveMesh(x, y, x - m_lastX, y - m_lastY, 100);
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
    draw();
    m_fbo->unbind();
}

//设置图片大小，不同于坐标
void MagicEngine::setSize( int w, int h )
{
    m_width = w;
    m_height = h;
    m_OutTex.setSize(w, h);
    if (m_sizeChange){
        m_sizeChange->OnOutputSizeChange(w, h);
    }
}

void MagicEngine::restore()
{
    m_Mesh->restore();
}

void MagicEngine::tackPicture(Texture *texutre /*= NULL*/)
{
    int ow = m_width, oh = m_height;
    m_fbo->bind();
    if (texutre){
        setSize(texutre->getWidth(), texutre->getHeight());
        draw(texutre);
    }else{
        setSize(m_InTex->getWidth(), m_InTex->getHeight());
        draw();
    }

    GLubyte* pixels = new GLubyte[m_width*m_height*4];

    glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    if(m_saveImage){
        m_saveImage->SaveImage((char*)pixels, m_width, m_height, 0);
    }
    delete[] pixels;
    checkGlError("SavePicture");
    m_fbo->unbind();

    setSize(ow, oh);
}

void MagicEngine::tackPicture( const char* data, long len )
{
    Texture tex;
    tex.uploadImageData((unsigned char*)data, len);
    tackPicture(&tex);
}

void MagicEngine::tackPicture( const char* imagePath )
{
    Texture tex;
    tex.loadFromFile(imagePath);
    tackPicture(&tex);
}

void MagicEngine::tackPicture( const char* data, int w, int h, int format )
{
    //TODO 暂时用不到，有时间完善 
}

void MagicEngine::draw(Texture *texutre /*= NULL*/)
{
    glViewport(0,0, m_width, m_height);
    m_shader->use();
    m_shader->setViewProj(m_vp);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(m_shader->getPositionLoc());
    glEnableVertexAttribArray(m_shader->getTextureCoordLoc());
    if (texutre){
        texutre->bind();
    }else{
        m_InTex->bind();
    }
    glDisable(GL_BLEND);
    m_Mesh->draw(m_shader);

    glBlendFunc(m_sfactor, m_dfactor);
}

void MagicEngine::setBlendFunc( GLenum sfactor, GLenum dfactor )
{
    m_sfactor = sfactor;
    m_dfactor = dfactor;
}

Texture* MagicEngine::getOutTexture()
{
    return &m_OutTex;
}

