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
    m_program = createProgram(pVertexSource, pFragmentSource);
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
        glDeleteShader(m_vertexShader);
        glDeleteShader(m_pixelShader);
        glDeleteProgram(m_program);
        m_isCompiled = false;
        m_program = 0;
        m_vertexShader = 0;
        m_pixelShader = 0;
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


GLuint BaseShader::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                        shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint BaseShader::createProgram( const char* pVertexSource, const char* pFragmentSource )
{
    m_vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!m_vertexShader) {
        return 0;
    }

    m_pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!m_pixelShader) {
        deleteProgram();
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, m_vertexShader);
        checkGlError("AttachVertexShader");
        glAttachShader(program, m_pixelShader);
        checkGlError("AttachPixelShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}
