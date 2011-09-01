#pragma once

#include <GLES2/gl2.h>
#include "glutils/texture.h"
#include "glutils/glyuvtexture.h"
#include "glutils/baseshader.h"
#include "glutils/sprite.h"
#include "ui/button.h"
#include "ui/touchevent.h"
#include "meshengine.h"
#include "utils/resmanage.h"

const int IMAGE_FORMAT_RGB_565    = 0x00000004; //4
const int IMAGE_FORMAT_NV21        = 0x00000011; //17
const int IMAGE_FORMAT_PACKET    = 0x00000100; //256  打包压缩的数据，jpeg,png,tga,bitmap...


const int FORMAT_RGBA = 0;
const int FORMAT_RGB = 1;
const int FORMAT_RGB565 = 2;

enum Status {
    Status_Normal, Status_Changing_Engine, Status_Tack_Photo
};


class MagicMain : public ButtonClick, TouchEvent{
    BaseShader      m_shader;
    Status          m_status;

    Texture*        m_SrcTex;
    glYUVTexture*   m_glYUVTex;

    MagicEngine*    m_Engine;
    EngineType      m_nextEngine;
    Button*         m_btn_func;
    Button*         m_btn_switch;
    Button*         m_BtnSave;

    Sprite          m_magicSprite;
    GLfloat         m_magicSpriteY;

    GLfloat         m_CoordWidth;
    GLfloat         m_CoordHeight;
    GLfloat         m_ScreenWidth;
    GLfloat         m_ScreenHeight;
    GLfloat         m_vp[16];

    int             m_inputFortmat;

    IOCallBack*      m_ioCallBack;

public:
    MagicMain();
    ~MagicMain();

    bool setupGraphics(int w, int h) ;
    void renderFrame(float delta);

    void updatePreviewData(char* data, long len);
    void setPreviewDataInfo(int w, int h, int imageFormat = IMAGE_FORMAT_NV21);

    void setPreviewImage(const char* data, long len);
    void setPreviewImage(const char* imgPath);

    void takePicture();

    EngineType getEngineType();
    void switchEngine(EngineType type);

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchDrag(float x, float y);
    virtual bool onTouchUp(float x, float y);

    void setIOCallBack(IOCallBack* callback);

    void setResPath(const char* path){::setResPath(path);};
    void loadRes();

private:
    void initEngine(EngineType type = EngineType_Mesh);
    
    virtual void onButtonClick( Button *btn );

    void update(float delta);

    void drawUI();
    void drawImage();

    virtual void OnOutputSizeChange();
};
