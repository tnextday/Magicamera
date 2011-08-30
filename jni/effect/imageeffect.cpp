#include "imageeffect.h"
#include <zlib.h>
#include <stdio.h>

static const GLfloat G_QuadData[] = {
    // X, Y, U, V
    -1.0f, -1.0f, 0.0f, 1.0f,
    +1.0f, -1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, 1.0f, 0.0f,
    -1.0f, +1.0f, 0.0f, 0.0f,
};

ImageEffect::ImageEffect(void)
{
}

ImageEffect::~ImageEffect(void)
{
    SafeDelete(m_fbo);
}

bool ImageEffect::loadFromMemory( const char* buf, int size )
{
    if (!buf || size < 12) return false;
    char* buffer = (char *)buf;
    if (buffer[0] != 'S' || buffer[1] != 'P' || buffer[2] != 'V' || buffer[3] != '1')
        return false;
    buffer +=4;
    int oSize = *((int*)buffer);
    buffer +=4;
    int zSize = *((int*)buffer);
    if (zSize > (size - 12)) return false;
    buffer +=4;
    char* oBuffer = new char[oSize];
    uLong destLen = oSize;
    int vsSize, fsSize;
    char* vs;
    char* fs;
    bool ret = false;
    if (Z_OK == uncompress((Bytef*)oBuffer, &destLen, (Bytef*)buffer, zSize)){
        vsSize = *((int *)oBuffer);
        vs = oBuffer + 4;
        fsSize = *((int *)(oBuffer + 4 + vsSize));
        fs = oBuffer + 8 + vsSize;
        ret = m_shader.makeProgram(vs,fs);
    }
    delete [] oBuffer;
    return ret;
}

bool ImageEffect::loadFromFile( const char* fileName )
{
    bool result = true;
    FILE *fp = fopen(fileName, "rb");
    if (!fp) {
        LOGE("Can't open file: %s\n", fileName);
        return false;
    }
    char* buffer = NULL;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);            // determine file size so we can fill it in later if FileSize == 0
    if (size <= 0) {
        fclose(fp);
        return false;
    }
    fseek(fp, 0, SEEK_SET);
    buffer = new char[size];

    if (fread(buffer, sizeof(char), size, fp) > 0){
        result = loadFromMemory(buffer, size);
    }else{
        LOGE("Can't load texture: %s\n", fileName);
        result = false;
    }
    fclose(fp);
    if (buffer){
        delete[] buffer;
        buffer = NULL;
    }
    return result;
}

void ImageEffect::doEffect()
{
    m_shader.use();
    glDisable(GL_DEPTH_TEST);
    m_fbo->bind();
    //printGLColorSpaceInfo();
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //m_InTex->bind(1);
    glUniform1i(m_imgTexLoc, 1);

    onDoEffect();

    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1); //话说不关掉这个就会报错
    glVertexAttribPointer(m_shader.getPositionLoc(), 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_fbo->unbind();
    checkGlError("ImageEffect::doEffect");
}

void ImageEffect::setInputTex( Texture * tex )
{
    m_InTex = tex;
    m_width = m_InTex->getWidth();
    m_height = m_InTex->getHeight();
    m_OutTex.setSize(m_width, m_height);
    onResize();
}

void ImageEffect::Init()
{
    m_width = 0;
    m_height = 0;
    m_OutTex.init();
    m_fbo = new FramebufferObject();
    m_fbo->texture2d(m_OutTex.getTexHandle());
    onInit();
}