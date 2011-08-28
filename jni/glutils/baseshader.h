#pragma once
#ifndef _baseshader_h_
#define _baseshader_h_

#include <GLES2/gl2.h>
#include "glutils.h"

class BaseShader
{
private:
    GLuint  m_program;
    GLuint  m_positionLoc;
    GLuint  m_texCoordLoc;
    GLuint  m_viewprojLoc;

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
    bool loadFromFile(const char* fileName);
    bool loadFromMemory(const char* buf, int size);
    void deleteProgram();

    GLuint getProgram();
    virtual void use();
    inline GLuint getPositionLoc();
    inline GLuint getTextureCoordLoc();
    inline GLuint getViewPorjLoc();
    void setViewProject(GLfloat *mvp);
    inline bool isCompiled();
    void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar);
    void setViewProj(GLfloat* mvp);
};
#endif // _baseshader_h_
