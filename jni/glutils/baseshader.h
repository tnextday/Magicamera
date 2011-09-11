#pragma once
#ifndef _baseshader_h_
#define _baseshader_h_

#include <GLES2/gl2.h>
#include "glutils.h"

class BaseShader
{
private:
    GLuint m_program;
    GLint  m_positionLoc;
    GLint  m_texCoordLoc;
    GLint  m_viewprojLoc;

    bool    m_isCompiled;

public:
    /**
     * 简单的ShaderProgram类，目前只支持固定的关键字(uMVPMatrix, aPosition,aTexCoord)
     * TODO 动态属性支持
     */
    BaseShader(void);
    BaseShader(const char* pVertexSource, const char* pFragmentSource);
    virtual ~BaseShader(void);

    virtual bool makeProgram(const char* pVertexSource, const char* pFragmentSource);

    void deleteProgram();

    virtual void use(){if (m_isCompiled) glUseProgram(m_program);};
    GLuint getProgram(){return m_program;};
    GLuint getPositionLoc(){return m_positionLoc;};
    GLuint getTextureCoordLoc(){return m_texCoordLoc;};
    GLuint getViewPorjLoc();
    void setViewProject(GLfloat *mvp);
    bool isCompiled(){ return m_isCompiled;};
    void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar);
    void setViewProj(GLfloat* mvp);

    bool loadFromRes(const char* fileName);
    bool loadFromMemory(const char* buf, int size);
};
#endif // _baseshader_h_
