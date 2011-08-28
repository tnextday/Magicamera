#include "baseshader.h"
#include "glutils.h"
#include "utils/mathelpers.h"

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
    m_texCoordLoc = glGetAttribLocation(m_program, "aTexCoord");
    m_viewprojLoc = glGetUniformLocation(m_program, "uMVPMatrix");
    m_isCompiled = true;
    return true;
}


GLuint BaseShader::getViewPorjLoc()
{
    return m_viewprojLoc;
}

void BaseShader::setViewProject( GLfloat *mvp )
{
    glUniformMatrix4fv(m_viewprojLoc, 1, GL_FALSE, (GLfloat*)mvp);
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
