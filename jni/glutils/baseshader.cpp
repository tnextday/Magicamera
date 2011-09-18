#include "baseshader.h"
#include "glutils.h"
#include "utils/mathelpers.h"
#include "utils/fileutils.h"
#include <zlib.h>


BaseShader::BaseShader(void)
{
    m_isCompiled = false;
    m_program = 0;
    m_positionLoc = -1;
    m_texCoordLoc = -1;
    m_viewprojLoc = -1;
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
    m_texCoordLoc = glGetAttribLocation(m_program, "aTexCoord");
    m_viewprojLoc = glGetUniformLocation(m_program, "uMVPMatrix");
    m_isCompiled = true;
    checkGlError("makeProgram");
    return true;
}


GLint BaseShader::getViewPorjLoc()
{
    return m_viewprojLoc;
}

void BaseShader::setViewProject( GLfloat *mvp )
{
    if (m_viewprojLoc >= 0)
        glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);
}


void BaseShader::ortho( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar )
{
    if (m_viewprojLoc < 0) return;
    GLfloat mvp[16];
    matIdentity(mvp);
    matOrtho(mvp, left, right, bottom, top, znear, zfar);
    use();
    glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);
}

void BaseShader::setViewProj( GLfloat* mvp )
{
    if (m_viewprojLoc < 0) return;
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

bool BaseShader::loadFromRes( const char* fileName )
{
    bool ret = false;
    uint32_t size;
    unsigned char* date = readRes(fileName, size);
    ret = loadFromMemory((char*)date, size);
    SafeDeleteArray(date);
    return ret;
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

GLint BaseShader::getUniformLoc( const char * uName )
{
    use();
    return glGetUniformLocation(m_program, uName);
}