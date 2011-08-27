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
     * �򵥵�ShaderProgram�࣬Ŀǰֻ֧�̶ֹ��Ĺؼ���(uMVPMatrix, aPosition,aTexCoord)
     * TODO ��̬����֧��
     */
    BaseShader(void);
    BaseShader(const char* pVertexSource, const char* pFragmentSource);
    ~BaseShader(void);

    bool makeProgram(const char* pVertexSource, const char* pFragmentSource);
    bool loadFromFile(const char* fileName);
    bool loadFromMemory(const char* buf, int size);
    void deleteProgram();

    GLuint getProgram();
    void use();
    GLuint getPositionLoc();
    GLuint getTextureCoordLoc();
    GLuint getViewPorjLoc();
    void setViewProject(GLfloat *mvp);
    bool isCompiled();
    void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar);
    void setViewProj(GLfloat* mvp);
};
#endif // _baseshader_h_
