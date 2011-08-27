#include "baseshader.h"
#include "glutils.h"
#include "utils/mathelpers.h"
#include <zlib.h>

BaseShader::BaseShader(void)
{
    m_isCompiled = false;
    m_positionLoc = 0;
    m_program = 0;
    m_texCoordLoc = 0;
    m_viewprojLoc = 0;
}

BaseShader::BaseShader( const char* pVertexSource, const char* pFragmentSource )
{
    makeProgram(pVertexSource, pFragmentSource);
}
BaseShader::~BaseShader(void)
{
    deleteProgram();
}

bool BaseShader::makeProgram( const char* pVertexSource, const char* pFragmentSource )
{
    deleteProgram();
    m_isCompiled = false;
    m_program = ::createProgram(pVertexSource, pFragmentSource);
    if (!m_program) {
        LOGE("Could not create program.\n");
        return false;
    }
    glUseProgram(m_program);

    m_positionLoc = glGetAttribLocation(m_program, "aPosition");
    checkGlError("glGetAttribLocation aPosition");
    if (m_positionLoc == -1) {
        LOGE("Could not get attrib location for aPosition");
        return false;
    }
    m_texCoordLoc = glGetAttribLocation(m_program, "aTexCoord");
    checkGlError("glGetAttribLocation aTexCoord");
    if (m_texCoordLoc == -1) {
        LOGE("Could not get attrib location for aTexCoord");
        return false;
    }

    m_viewprojLoc = glGetUniformLocation(m_program, "uMVPMatrix");
    checkGlError("glGetUniformLocation uMVPMatrix");
    if (m_viewprojLoc == -1) {
        LOGE("Could not get attrib location for uMVPMatrix");
        return false;
    }
    m_isCompiled = true;
    return true;
}

GLuint BaseShader::getProgram()
{
    return m_program;
}

void BaseShader::use()
{
    if (isCompiled())
        glUseProgram(m_program);
}

GLuint BaseShader::getPositionLoc()
{   
    return m_positionLoc;
}

GLuint BaseShader::getTextureCoordLoc()
{
    return m_texCoordLoc;
}

GLuint BaseShader::getViewPorjLoc()
{
    return m_viewprojLoc;
}

void BaseShader::setViewProject( GLfloat *mvp )
{
    glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);
}

bool BaseShader::isCompiled()
{
    return m_isCompiled;
}

void BaseShader::ortho( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar )
{
    GLfloat mvp[16];
    matIdentity(mvp);
    matOrtho(mvp, left, right, bottom, top, znear, zfar);
    glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);
}

void BaseShader::setViewProj( GLfloat* mvp )
{
    glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);
}

void BaseShader::deleteProgram()
{
    if(m_program){
        glDeleteProgram(m_program);
        m_isCompiled = false;
        m_program = 0;
    }
}

bool BaseShader::loadFromMemory( const char* buf, int size )
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
        ret = makeProgram(vs,fs);
    }
    delete [] oBuffer;
    return ret;
}

bool BaseShader::loadFromFile( const char* fileName )
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