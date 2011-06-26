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
     * �򵥵�ShaderProgram�࣬Ŀǰֻ֧�̶ֹ��Ĺؼ���(uMVPMatrix, aPosition,aTextureCoord)
     * TODO ��̬����֧��
     */
    BaseShader(void);
    BaseShader(const char* pVertexSource, const char* pFragmentSource);
    ~BaseShader(void);

    void makeProgram(const char* pVertexSource, const char* pFragmentSource);

    GLuint getProgram();
    void use();
    GLuint getPositionLoc();
    GLuint getTextureCoordLoc();
    GLuint getViewPorjLoc();
    void setViewProject(GLfloat *mvp);
    bool isCompiled();
    void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar);
};
#endif // _baseshader_h_
