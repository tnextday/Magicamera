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
    m_saveImagePath[0] = '\0';
}

MagicEngine::~MagicEngine()
{
    SafeDelete(m_Mesh);
    SafeDelete(m_PreviewTex);
    SafeDelete(m_glYUVTex);
    SafeDelete(m_fbo);
    glDeleteProgram(m_Program);
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
//      glCullFace(GL_FRONT);
//      glEnable(GL_CULL_FACE);
    //启用混合操作
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_ViewWidth = w;
    m_ViewHeight = h;

    glViewport(0, 0, m_ViewWidth, m_ViewHeight);

    //使用2D投影,笛卡尔坐标系，宽高为屏幕宽高
    GLfloat mvp[16];
    matIdentity(mvp);
    // 设置视口的大小
    matOrtho(mvp, 0, w, 0, h, -10, 10);
    //设置镜头
    //      matLookAt((float*)mat_t, 0,0, -0.5, 0,0,0, 0,1,0);
    //      matMult((float*)mvp, (float*)mat_p, (float*)mat_t);    
    glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);

    m_PreviewTex = new Texture();
    m_fbo = new FramebufferObject();
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
    glVertexAttribPointer(m_texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void MagicEngine::renderFrame( float delta )
{
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    drawImage(delta);
    drawUI(delta);
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

void printGLInfo(){
    GLint param;
    glGetIntegerv(GL_RED_BITS, &param);//缓冲red位数
    LOGI("Red bits: %d\n", param);
    glGetIntegerv(GL_GREEN_BITS, &param);//缓冲green位数
    LOGI("Green bits: %d\n", param);
     glGetIntegerv(GL_BLUE_BITS, &param);
    LOGI("Blue bits: %d\n", param);
    glGetIntegerv(GL_ALPHA_BITS, &param);//缓冲Alpha位数
    LOGI("Alpha bits: %d\n", param);
}

void MagicEngine::makePicture( int w, int h )
{
    char path[_MAX_PATH];
    glDisable(GL_DEPTH_TEST);
    m_fbo->resizeColorBuffer(w, h);
    printGLInfo();
    m_fbo->bind();
    printGLInfo();
    glViewport(0,0, w, h);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    checkGlError("makePicture_0");
    glClear(GL_COLOR_BUFFER_BIT);
    checkGlError("makePicture_1");
    drawImage(0);
    checkGlError("makePicture_2");
    GLubyte* pixels = new GLubyte[w*h*2];
    glReadPixels(0, 0, w, h, GL_RGB565, GL_UNSIGNED_BYTE, pixels);
    snprintf(path, _MAX_PATH-1, "%s/test.tga", m_saveImagePath);
    saveImage(pixels, w, h, path);
    delete[] pixels;
    m_fbo->unbind();
    glViewport(0, 0, m_ViewWidth, m_ViewHeight);

}

void MagicEngine::drawUI( float delta )
{

}

void MagicEngine::drawImage( float delta )
{
    glUseProgram(m_Program);
    m_Mesh->update(delta);
    m_PreviewTex->bind();
    m_Mesh->draw();
}

bool MagicEngine::saveImage( GLubyte* buffer, int w, int h, char* filename )
{
    struct tgaheader_t
    {
        GLubyte   idLength;
        GLubyte   colorMapType;
        GLubyte   imageType;
        GLubyte   colorMapSpec[5];
        GLushort  xOrigin;
        GLushort  yOrigin;
        GLushort  width;
        GLushort  height;
        GLubyte   bpp;
        GLubyte   imageDesc;
    };
    struct rgb565_t
    {
        GLushort
            b : 5,
            g : 6,
            r : 5;
    };

#define TGA_RGB 2
    FILE* pFile;

    pFile = fopen(filename, "wb");
    if (!pFile)	return false;

    // read in the image type
    tgaheader_t tga;		// TGA header
    memset(&tga, 0, sizeof(tgaheader_t));
    tga.bpp = 16;
    tga.height = h;
    tga.width = w;
    tga.imageType = TGA_RGB;
    long szData = w*h*2;

//     GLubyte temp;
//     int total = w * h;
//     rgb565_t* source = (rgb565_t*)buffer;
//     for (int pixel = 0; pixel < total; ++pixel)
//     {
//         temp = source[pixel].b;
//         source[pixel].b = source[pixel].r;
//         source[pixel].r = temp;
//     }

    fwrite(&tga, sizeof(tgaheader_t), 1, pFile);
    fwrite(buffer, sizeof(GLubyte), szData, pFile);
    if (pFile) fclose(pFile);
    return true;
}

void MagicEngine::setSaveImagePath( char* path )
{
    snprintf(m_saveImagePath, _MAX_PATH-1, "%s", path);
}