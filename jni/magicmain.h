#pragma once

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

const static int g_ViewWidth = 480;

const int IMAGE_FORMAT_RGB_565    = 0x00000004; //4
const int IMAGE_FORMAT_NV21        = 0x00000011; //17
const int IMAGE_FORMAT_PACKET    = 0x00000100; //256  打包压缩的数据，jpeg,png,tga,bitmap...

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif
#ifdef _WIN32
#define snprintf _snprintf
#endif

const int FORMAT_RGBA = 0;
const int FORMAT_RGB = 1;
const int FORMAT_RGB565 = 2;

class SaveImageCallBack{
public:
    virtual bool SaveImage(char* buffer, int w, int h, int format) = 0;
};

class MagicMain : public ButtonClick{
    BaseShader      m_shader;

    Texture*        m_PreviewTex;
    glYUVTexture*   m_glYUVTex;
    MeshEngine*     m_Mesh;
    Button*         m_testBtn;

    Sprite          m_testSprite;
    Texture         m_testTexture;

    GLfloat         m_ViewWidth;
    GLfloat         m_ViewHeight;
    GLfloat         m_ScreenWidth;
    GLfloat         m_ScreenHeight;

    int             m_inputFortmat;

    //上次鼠标坐标
    float    m_lastX;
    float    m_lastY;

    FramebufferObject*      m_fbo;
    char                    m_resPath[_MAX_PATH];
    SaveImageCallBack*      m_saveImage;

public:
    MagicMain();
    ~MagicMain();

    bool setupGraphics(int w, int h) ;
    void renderFrame(float delta);

    void updatePreviewTex(char* data, long len);
    void setPreviewDataInfo(int w, int h, int imageFormat = IMAGE_FORMAT_NV21);

    void generateMesh( int w, int h );
    bool onTouchDown(float x, float y);
    bool onTouchDrag(float x, float y);
    bool onTouchUp(float x, float y);

    void makePicture(int w, int h);

    void update(float delta);

    void drawUI();
    void drawImage();

    void setCallBack(SaveImageCallBack* callback);

    void setResPath(const char* path);

    void loadRes();

    char* makeResPath(char* path, const char* targetFile, int szBuffer = _MAX_PATH);

    virtual void onButtonClick( Button *btn );
};


#endif // _glengine_h_