#include "baseshader.h"
#include "glutils.h"

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
    if(m_program){
        glDeleteProgram(m_program);
    }
}

void BaseShader::makeProgram( const char* pVertexSource, const char* pFragmentSource )
{
    m_isCompiled = false;
    m_program = ::createProgram(pVertexSource, pFragmentSource);
    if (!m_program) {
        LOGE("Could not create program.\n");
        return;
    }
    glUseProgram(m_program);

    m_positionLoc = glGetAttribLocation(m_program, "aPosition");
    checkGlError("glGetAttribLocation aPosition");
    if (m_positionLoc == -1) {
        LOGE("Could not get attrib location for aPosition");
        return;
    }
    m_texCoordLoc = glGetAttribLocation(m_program, "aTextureCoord");
    checkGlError("glGetAttribLocation aTextureCoord");
    if (m_texCoordLoc == -1) {
        LOGE("Could not get attrib location for aTextureCoord");
        return;
    }

    m_viewprojLoc = glGetUniformLocation(m_program, "uMVPMatrix");
    checkGlError("glGetUniformLocation uMVPMatrix");
    if (m_viewprojLoc == -1) {
        LOGE("Could not get attrib location for uMVPMatrix");
        return;
    }
    m_isCompiled = true;
}

GLuint BaseShader::getProgram()
{
    return m_program;
}

void BaseShader::useProgram()
{
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