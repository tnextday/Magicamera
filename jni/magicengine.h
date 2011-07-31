#pragma once

#include <GLES2/gl2.h>
#include "meshengine.h"
#include "texture.h"
#include "baseshader.h"
#include "touchevent.h"
#include "framebufferobject.h"
#include "sprite.h"

//��߱�Ϊ2:3
const static int MESH_WIDTH = 50;
const static int MESH_HEIGHT = MESH_WIDTH*3/2;

//MagicEngine ����ϵ��С
const static int g_CoordWidth = 480;
const static int g_CoordHeight = 640;

class SaveImageCallBack{
public:
    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
};

class OutputSizeChangeCallBack{
public:
    virtual void OnOutputSizeChange(int w, int h) = 0;
};

class MagicEngine : public TouchEvent {
private:
    BaseShader*     m_shader;
    BaseShader      m_effectShader;

    Texture*        m_InTex;

    Texture         m_OutTex;
    Texture         m_coverTex;
    Sprite          m_srcImg;
    

    MeshEngine*     m_Mesh;

    GLfloat         m_width;
    GLfloat         m_height;
    GLfloat         m_coordWidth;
    GLfloat         m_coordHeight;
    GLfloat         m_vp[16]; 

    GLenum          m_sfactor;
    GLenum          m_dfactor;

    //�ϴ��������
    float    m_lastX;
    float    m_lastY;

    FramebufferObject*      m_fbo;
    SaveImageCallBack*      m_saveImage;
    OutputSizeChangeCallBack* m_sizeChange;

public:
    MagicEngine();
    MagicEngine(BaseShader* shader, Texture* srcTex);
    ~MagicEngine();

    bool init(BaseShader* shader, Texture* srcTex) ;
    void setSize(int w, int h);
    void setShader(BaseShader* val) { m_shader = val; }
    void setInputTexture(Texture* val) { m_InTex = val; }
    Texture* getOutTexture();

    void restore();

    void generateMesh( int w, int h );

    void update(float delta);

    void drawImage();

    void tackPicture(Texture *texutre = NULL);
    void tackPicture(const char* data, long len);
    void tackPicture(const char* imagePath);
    void tackPicture(const char* data, int w, int h, int format);

    void SetSaveImageCallBack(SaveImageCallBack* val) { m_saveImage = val; }
    void SetSizeChangeCallBack(OutputSizeChangeCallBack *val){ m_sizeChange = val;}

    void setBlendFunc(GLenum sfactor, GLenum dfactor);

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchDrag(float x, float y);
    virtual bool onTouchUp(float x, float y);

private:
    void draw(Texture *texutre = NULL);
    //����
    void drawMesh(Texture *texutre);
    //���
    void drawCover(Texture *texutre);
    //��Ͳ
    void drawKaleidoscope(Texture *texutre);

};
