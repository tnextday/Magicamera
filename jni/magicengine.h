#ifndef _glengine_h_
#define _glengine_h_

#include <GLES2/gl2.h>
#include "meshengine.h"
#include "texture.h"
#include "glyuvtexture.h"
#include "baseshader.h"
#include "sprite.h"
#include "button.h"

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

class MagicEngine {
    BaseShader*     m_shader;


    Texture*        m_SrcTex;
    Texture*        m_DestTex;

    MeshEngine*     m_Mesh;

    GLfloat         m_width;
    GLfloat         m_height;
    GLfloat         m_vp[16]; 

    //上次鼠标坐标
    float    m_lastX;
    float    m_lastY;

    FramebufferObject*      m_fbo;

public:
    MagicEngine();
    ~MagicEngine();

    bool init(BaseShader* shader, Texture* srcTex) ;
    void setSize(int w, int h);
    void setShader(BaseShader* val) { m_shader = val; }
    void setSrcTex(Texture* val) { m_SrcTex = val; }
    Texture* getOutTexture(){return m_DestTex;};

    void generateMesh( int w, int h );
    bool onTouchDown(float x, float y);
    bool onTouchDrag(float x, float y);
    bool onTouchUp(float x, float y);

    void update(float delta);

    void drawImage();

};


#endif // _glengine_h_