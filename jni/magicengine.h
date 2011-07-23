#pragma once

#include <GLES2/gl2.h>
#include "meshengine.h"
#include "texture.h"
#include "baseshader.h"
#include "touchevent.h"
#include "framebufferobject.h"
#include "sprite.h"

//宽高比为2:3
const static int MESH_WIDTH = 50;
const static int MESH_HEIGHT = MESH_WIDTH*3/2;

//MagicEngine 坐标系大小
const static int g_CoordWidth = 480;
const static int g_CoordHeight = 640;

class SaveImageCallBack{
public:
    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
};

class MagicEngine : public TouchEvent {
    BaseShader*     m_shader;

    Texture*        m_InTex;
    Texture*        m_OutTex;

    MeshEngine*     m_Mesh;

    GLfloat         m_width;
    GLfloat         m_height;
    GLfloat         m_vp[16]; 

    //上次鼠标坐标
    float    m_lastX;
    float    m_lastY;

    FramebufferObject*      m_fbo;
    SaveImageCallBack*      m_saveImage;

    Sprite          m_testSprite;

public:
    MagicEngine();
    MagicEngine(BaseShader* shader, Texture* srcTex);
    ~MagicEngine();

    bool init(BaseShader* shader, Texture* srcTex) ;
    void setSize(int w, int h);
    void setShader(BaseShader* val) { m_shader = val; }
    void setInTexture(Texture* val) { m_InTex = val; }
    Texture* getOutTexture(){return m_OutTex;};

    void restore();

    void generateMesh( int w, int h );

    void update(float delta);

    void drawImage();

    void SaveImage(int w, int h);
    void SetSaveImageCallBack(SaveImageCallBack* val) { m_saveImage = val; }

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchDrag(float x, float y);
    virtual bool onTouchUp(float x, float y);

private:
    void draw();

};
