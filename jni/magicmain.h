#pragma once

#include <GLES2/gl2.h>
#include "texture.h"
#include "glyuvtexture.h"
#include "baseshader.h"
#include "sprite.h"
#include "button.h"
#include "touchevent.h"
#include "magicengine.h"

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



class MagicMain : public ButtonClick, TouchEvent, OutputSizeChangeCallBack{
    BaseShader      m_shader;

    Texture*        m_SrcTex;
    glYUVTexture*   m_glYUVTex;

    MagicEngine    m_Engine;
    Button*         m_BtnRestore;
    Button*         m_BtnSave;

    Sprite          m_magicSprite;
    GLfloat         m_magicSpriteY;

    GLfloat         m_CoordWidth;
    GLfloat         m_CoordHeight;
    GLfloat         m_ScreenWidth;
    GLfloat         m_ScreenHeight;
    GLfloat         m_vp[16];

    int             m_inputFortmat;

    char                    m_resPath[_MAX_PATH];
    SaveImageCallBack*      m_saveImage;

public:
    MagicMain();
    ~MagicMain();

    bool setupGraphics(int w, int h) ;
    void renderFrame(float delta);

    void updatePreviewData(char* data, long len);
    void setPreviewDataInfo(int w, int h, int imageFormat = IMAGE_FORMAT_NV21);

    void setPreviewImage(const char* data, long len);
    void setPreviewImage(const char* imgPath);

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchDrag(float x, float y);
    virtual bool onTouchUp(float x, float y);

    void setCallBack(SaveImageCallBack* callback);

    void setResPath(const char* path);
    void loadRes();
    char* makeResPath(char* path, const char* targetFile, int szBuffer = _MAX_PATH);

private:
    void initEngine();
    virtual void onButtonClick( Button *btn );

    void update(float delta);

    void drawUI();
    void drawImage();

    virtual void OnOutputSizeChange( int w, int h );
};
