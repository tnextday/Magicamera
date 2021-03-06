#pragma once

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


class MagicMain : public TouchEvent, EngineOutChange{
    BaseShader      m_shader;
    Status          m_status;

    Texture*        m_origSrcTex;
    Texture*        m_srcTex;
    glYUVTexture*   m_glYUVTex;

    MagicEngine*    m_Engine;
    EngineType      m_nextEngine;

    Sprite          m_outSprite;

    GLfloat         m_ScreenWidth;
    GLfloat         m_ScreenHeight;
    GLfloat         m_aspectRatio;
    GLuint          m_PreviewWidth;
    GLuint          m_PreviewHeight;

    GLuint          m_MaxWidth;
    GLuint          m_MaxHeight;

    int             m_inputFortmat;

    FileUtils*     m_ioCallBack;

    ImageAdjust     m_adjust;

public:
    MagicMain();
    ~MagicMain();

    bool setupGraphics() ;
    void renderFrame(float delta);

    void updateInputData(char* data, long len);
    void setInputDataInfo(int w, int h, int imageFormat = IMAGE_FORMAT_NV21);

    void setInputImage(const char* data, long len);
    void setInputImage(const char* imgPath);

    void takePicture(Texture *tex = NULL);
    void takePicture(const char* imgPath);
    void takePicture(const char* data , long len);

    EngineType getEngineType();
    void switchEngine(EngineType type);

/*    void setCover(const unsigned char* buffer, uint32_t size);*/
    void setFrame(const char* path);
    void setCover(const char* path);

    const char* getEffectList();
    void setEffect(const char* name);
    const char* getEffectName();
    void setParameter(const char* parameterKey, float value);
    float getParameterValue(const char* parameterKey);
    const char* getParameterKeys();

    void restoreMesh();

    void rotate90Input(bool clockwise = true);
    void flip(bool x , bool y );
    void resetRotation();

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchDrag(float x, float y);
    virtual bool onTouchUp(float x, float y);

    void setIOCallBack(FileUtils* callback);

    //Engine输出texture大小改变 
    virtual void onEngineOutChange( Texture *tex );

    void resize(int w, int h);
    void setPreviewSize(GLuint w, GLuint h);

    void setMaxOutputSize(int w, int h);

private:
    void initEngine(EngineType type = EngineType_Effect);

    void update(float delta);

    void drawImage();
    //更新旋转状态
    void updateRotation();

    //将输入坐标转换成笛卡尔坐标
    GLfloat transformX(float x){return m_aspectRatio*(x/m_ScreenWidth - 1.0/2.0);}
    GLfloat transformY(float y){return 1.0/2.0 - y/m_ScreenHeight;}


};
