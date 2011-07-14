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
    m_resPath[0] = '\0';
    m_saveImage = NULL;

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

    m_ViewWidth = w;
    m_ViewHeight = h;
    m_PreviewTex = new Texture();
    m_PreviewTex->init();
    m_fbo = new FramebufferObject(true);
    printGLColorSpaceInfo();
    resize(m_ViewWidth, m_ViewHeight);
    return true;
}


void MagicEngine::resize( int w, int h )
{
    //使用2D投影,笛卡尔坐标系，宽高为屏幕宽高
    m_shader.ortho(0, w, 0, h, -10, 10);
    glViewport(0, 0, w, h);
    checkGlError("matOrtho");
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
    GLfloat texVertex[8] = {0};
    GLint w,h;
    w = tex->m_Width/2;
    h = tex->m_Height/2;

    texVertex[0] = posX-w; texVertex[1] = posY+h; 
    texVertex[2] = posX-w; texVertex[3] = posY-h; 
    texVertex[4] = posX+w; texVertex[5] = posY-h; 
    texVertex[6] = posX+w; texVertex[7] = posY+h;
    
    tex->bind(); 
    glVertexAttribPointer(m_shader.getPositionLoc(), 2, GL_FLOAT, GL_FALSE, 0, texVertex);
    glVertexAttribPointer(m_shader.getTextureCoordLoc(), 2, GL_FLOAT, GL_FALSE, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
    drawUI();
    checkGlError("renderFrame");
}

void MagicEngine::updatePreviewTex( char* data, long len )
{
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        //m_PreviewTex->uploadImageData((GLubyte*)m_tmpImageData);
        m_glYUVTex->uploadYUVTexImage(data, m_PreviewTex->m_Width, m_PreviewTex->m_Height);
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

    //rgb565比rgb888快至少30%
    if (m_inputFortmat == IMAGE_FORMAT_NV21){
        m_glYUVTex = new glYUVTexture(w, h, m_PreviewTex->m_TexHandle);
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
    if (x > m_ViewWidth - 50 && y > m_ViewHeight -50){
        m_Mesh->restore();
    }else if(x > m_ViewWidth - 50 && y < 50){
        makePicture(640, 480);
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
    //y = m_ViewHeight - y;
    m_lastX = 0;
    m_lastY = 0;
    return true;
}

void MagicEngine::setCallBack( SaveImageCallBack* callback )
{
    m_saveImage = callback;
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
    m_shader.ortho(0, 640, 0, 480, -10, 10);
    printGLColorSpaceInfo();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    checkGlError("makePicture_0");
    glClear(GL_COLOR_BUFFER_BIT);
    checkGlError("makePicture_1");
    drawImage();
    checkGlError("makePicture_2");
    GLubyte* pixels = new GLubyte[w*h*4];
//    GLint format, type;
//     glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &format);
//     glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &type);
    
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    if(m_saveImage){
        m_saveImage->SaveImage((char*)pixels, w, h, FORMAT_RGBA);
    }
    delete[] pixels;
    m_fbo->unbind();
    resize(m_ViewWidth, m_ViewHeight);
}

void MagicEngine::update( float delta )
{
    m_Mesh->update(delta);
    static float rotateSpeed = 50;
    static float scaleSpeed = 1.5;
    m_testSprite.rotate(rotateSpeed*delta);
    static float scale = 1.0;
    
    if (scale < 0.2 || scale > 1.0){
        scaleSpeed = -scaleSpeed;
    }
    scale += scaleSpeed*delta;
    m_testSprite.setScale(scale);
}

void MagicEngine::drawUI()
{

}

void MagicEngine::drawImage()
{
    m_shader.use();
    m_PreviewTex->bind();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_Mesh->draw(&m_shader);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_COLOR);
    m_testSprite.draw(&m_shader);
/*    drawTexture(m_PreviewTex, m_ViewWidth/2, m_ViewHeight/2);*/
}

void MagicEngine::setResPath(const char* path )
{
    snprintf(m_resPath, _MAX_PATH-1, "%s", path);
    LOGI("setSaveImagePath : %s\n", path);
}

void MagicEngine::loadRes()
{
    char path[_MAX_PATH];
    m_testTexture.loadFromFile(makeResPath(path, "sprite.png"));
    m_testSprite.setTexture(&m_testTexture);
    m_testSprite.setPostion(m_ViewWidth/2, m_ViewHeight/2);
}

char* MagicEngine::makeResPath( char* path, const char* targetFile, int szBuffer/* = _MAX_PATH*/)
{
    snprintf(path, szBuffer, "%s/%s", m_resPath, targetFile);
    return path;
}

