#ifndef _glengine_h_
#define _glengine_h_

#include <GLES2/gl2.h>
#include "meshengine.h"
#include "texture.h"
#include "glyuvtexture.h"

const static int MESH_HEIGHT = 50;

const int IMAGE_FORMAT_RGB_565    = 0x00000004; //4
const int IMAGE_FORMAT_NV21        = 0x00000011; //17
const int IMAGE_FORMAT_PACKET    = 0x00000100; //256  打包压缩的数据，jpeg,png,tga,bitmap...

class MagicEngine{
    GLuint m_Program;
    GLuint m_positionLoc;
    GLuint m_texCoordLoc;
    GLuint m_viewprojLoc;

    Texture*        m_PreviewTex;
    glYUVTexture*   m_glYUVTex;
    MeshEngine*     m_Mesh;

    GLfloat m_ViewWidth;
    GLfloat m_ViewHeight;
    
    int        m_inputFortmat;

    //上次鼠标坐标
    float    m_lastX;
    float    m_lastY;

    FramebufferObject *m_fbo;

public:
    MagicEngine();
    ~MagicEngine();

    bool setupGraphics(int w, int h) ;
    void renderFrame(float delta);

    void updatePreviewTex(char* data, long len);
    void setPreviewDataInfo(int w, int h, int imageFormat = IMAGE_FORMAT_NV21);

    void generateMesh( int w, int h );
    bool onTouchDown(float x, float y);
    bool onTouchDrag(float x, float y);
    bool onTouchUp(float x, float y);
    void drawTexture( Texture *tex, GLint posX, GLint posY );

    void makePicture(int w, int h);

    void drawUI(float delta);
    void drawImage(float delta);

    bool saveImage(char* buffer, int w, int h, char* filename);
};


#endif // _glengine_h_