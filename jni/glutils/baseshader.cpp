#include "baseshader.h"
#include "glutils.h"
#include "utils/mathelpers.h"
#include "utils/fileutils.h"
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>



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
    //matIdentity(mvp);
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

bool BaseShader::loadFromFile( const char* fileName )
{
    bool ret = false;
    uint32_t size;
    unsigned char* date = EasyReadFile(fileName, size);
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

void BaseShader::setAttrf( const char* attr_name, GLfloat value )
{
    GLint attr_loc = glGetAttribLocation(m_program, attr_name);
    if (attr_loc >= 0)
        glUniform1f(attr_loc, value);
}

//TODO 保存编译后的shader
//http://www.khronos.org/registry/gles/extensions/OES/OES_get_program_binary.txt

bool BaseShader::saveBinary( const char* fileName )
{
    if (checkIfSupportsExtension("GL_OES_get_program_binary"))
        return false;
#ifndef ANDROID
    return false;
#else
//    GLsizei binaryLength;
//    GLvoid* binary;
//    GLenum format;
//    FILE*   outfile;
//
//    //
//    //  Retrieve the binary from the program object
//    //
//    glGetProgramiv(m_program, GL_PROGRAM_BINARY_LENGTH_OES, &binaryLength);
//    binary = (GLvoid*)malloc(binaryLength);
//    glGetProgramBinaryOES(m_program, binaryLength, NULL, &format, binary);
//
//    //
//    //  Cache the program binary for future runs
//    //
//    outfile = fopen(fileName, "wb");
//    fwrite(binary, binaryLength, 1, outfile);
//    fclose(outfile);
//    free(binary);
    return true;
#endif

}

bool BaseShader::loadBinary( const char* binary, int size )
{
#ifndef ANDROID
    return false;
#else
//    GLint   success;
//    GLenum format = 1;//TODO 此处为binary格式，应该与glGetProgramBinaryOES中的format对应
//    glProgramBinaryOES(m_program, format, binary, size);
//    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
//    if (!success)
//    {
//        //
//        // Something must have changed since the program binaries
//        // were cached away.  Fallback to source shader loading path,
//        // and then retrieve and cache new program binaries once again.
//        //
//        return false;
//    }
    return true;
#endif

}