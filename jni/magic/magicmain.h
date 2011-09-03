#pragma once

#include <GLES2/gl2.h>
#include "glutils/texture.h"
#include "glutils/glyuvtexture.h"
#include "glutils/baseshader.h"
#include "glutils/sprite.h"
#include "ui/touchevent.h"
#include "meshengine.h"
#include "imageadjust.h"

const int IMAGE_FORMAT_RGB_565    = 0x00000004; //4
const int IMAGE_FORMAT_NV21        = 0x00000011; //17
const int IMAGE_FORMAT_PACKET    = 0x00000100; //256  打包压缩的数据，jpeg,png,tga,bitmap...


const int FORMAT_RGBA = 0;
const int FORMAT_RGB = 1;
const int FORMAT_RGB565 = 2;

enum Status {
    Status_Normal, Status_Changing_Engine, Status_Tack_Photo
};


class MagicMain : public TouchEvent, EngineOutChange, AdjustChange{
    BaseShader      m_shader;
    Status          m_status;

    Texture         m_SrcTex;
    glYUVTexture*   m_glYUVTex;

    MagicEngine*    m_Engine;
    EngineType      m_nextEngine;

    Sprite          m_magicSprite;
    GLfloat         m_magicSpriteY;

    GLfloat         m_CoordWidth;
    GLfloat         m_CoordHeight;
    GLfloat         m_ScreenWidth;
    GLfloat         m_ScreenHeight;
    GLfloat         m_vp[16];

    int             m_inputFortmat;

    IOCallBack*     m_ioCallBack;

    ImageAdjust     m_adjust;

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

    void setCover(const unsigned char* buffer, uint32_t size);
    void restoreMesh();

    void rotate90Input(bool clockwise = true);

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchDrag(float x, float y);
    virtual bool onTouchUp(float x, float y);

    void setIOCallBack(IOCallBack* callback);

    //Engine输出texture大小改变
    virtual void onEngineOutChange( Texture *tex );

    void resize(int w, int h);

private:
    void initEngine(EngineType type = EngineType_Mesh);

    void update(float delta);

    void drawImage();

    virtual void onAdjustChange( Texture *tex );
};
