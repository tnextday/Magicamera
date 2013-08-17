#include "glyuvtexture.h"
#include <string.h>

/*
* 使用GL shader 来进行YUV420SP2RGB转换，利用GPU强悍的浮点运算，转换速度为CPU的3倍以上
* 关于YUV420SP的储存结构，首先是w*h字节的y数据，然后为w*h/2字节的uv交替排列，每2个y公用一组UV
* 使用两个Texture传递数据给GPU，一个传递Y数据，另一个专递UV数据
* Y数据使用LUMINANCE传递，每个像素占一个字节，
* UV使用GL_LUMINANCE_ALPHA传递，每个像素占用两字节，在shader中使用rgb.ra来使用数据
* 这样可以直接使用GPU解析YUV420SP而不必进行内存拷贝
*/



static const char g_YUV_VShader[] = 
        "attribute vec4 aPosition;\n"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
        "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
        "}\n";

static const char g_YUV_FShader[] = 
        "precision mediump float;\n"
        "varying vec2 vTexCoord;\n"
        "uniform sampler2D Ytex;\n"
        "uniform sampler2D UVtex;\n"
        "void main() {\n"
        "    float r,g,b,y,u,v;\n"
        "    y = texture2D(Ytex, vTexCoord).r;\n"
        "    vec4 uv = texture2D(UVtex, vTexCoord);\n"
        "    y = 1.1643*(y-0.0625);\n"
        "    u = uv.a-0.5;\n"
        "    v = uv.r-0.5;\n"
        "    r = y+1.5958*v;\n"
        "    g = y-0.39173*u-0.81290*v;\n"
        "    b = y+2.017*u;\n"
        "    gl_FragColor = vec4(r,g,b,1.0);\n"
        "}\n";

static const GLfloat QuadData[] = {
    // X, Y, U, V
    -1.0f, -1.0f, 0.0f, 1.0f,
    +1.0f, -1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, 1.0f, 0.0f,
    -1.0f, +1.0f, 0.0f, 0.0f,
};

glYUVTexture::glYUVTexture(int w, int h, Texture *tex)
{
    m_fbo = NULL;
    init(w, h, tex);
}

glYUVTexture::~glYUVTexture(void)
{
    glDeleteTextures(2, m_YUVTexs);
    SafeDelete(m_fbo);
}

bool glYUVTexture::init( int w, int h, Texture *tex)
{
    m_QuadData = (GLfloat *)QuadData;
    m_Adjust = NULL;
    m_shader.makeProgram(g_YUV_VShader, g_YUV_FShader);
    if (!m_shader.isCompiled()) {
        LOGE("Could not create program.\n");
        return false;
    }
    m_shader.use();
    m_uYtexLoc = glGetUniformLocation(m_shader.getProgram(), "Ytex");
    m_uUVTexLoc = glGetUniformLocation(m_shader.getProgram(), "UVtex");

    glGenTextures(2, m_YUVTexs);
    setDefaultTexParameter(m_YUVTexs[YTexId_idx]);
    setDefaultTexParameter(m_YUVTexs[UVTexId_idx]);

    m_iWidth = w;
    m_iHeight = h;
    m_fbo = new FramebufferObject();
    m_oWidth = m_iWidth;
    m_oHeight = m_iHeight;
    setDstTex(tex);
    checkGlError("glYUVTexture::init");
    return true;
}

void glYUVTexture::setDefaultTexParameter( GLuint texId )
{
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void glYUVTexture::uploadYUVTexImage( char* yuv420sp)
{

    m_shader.use();
    glDisable(GL_DEPTH_TEST);
    m_fbo->bind();
    glViewport(0, 0, m_oWidth, m_oHeight);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_YUVTexs[YTexId_idx]);
    glUniform1i(m_uYtexLoc, 1);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,m_iWidth,m_iHeight,0,GL_LUMINANCE,GL_UNSIGNED_BYTE, yuv420sp);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_YUVTexs[UVTexId_idx]);
    glUniform1i(m_uUVTexLoc, 2);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,m_iWidth/2,m_iHeight/2,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE, yuv420sp+m_iWidth*m_iHeight);

    
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1); //话说不关掉这个就会报错
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, m_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_fbo->unbind();
    checkGlError("uploadYUVTexImage");
}

void glYUVTexture::setDstTex( Texture *tex )
{
    m_DstTex = tex;
    m_fbo->texture2d(tex->getTexHandle());
    m_DstTex->setSize(m_oWidth, m_oHeight);
}

void glYUVTexture::setImageAdjust( ImageAdjust *adjust )
{
    m_Adjust = adjust;
    if (m_Adjust){
        m_Adjust->setSize(m_iWidth, m_iHeight);
        m_oWidth = m_Adjust->getWidth();
        m_oHeight = m_Adjust->getHeight();
        m_QuadData = m_Adjust->getQuadData();
    }else{
        m_oWidth = m_iWidth;
        m_oHeight = m_iHeight;
        m_QuadData = (GLfloat *)QuadData;
    }
    m_DstTex->setSize(m_oWidth, m_oHeight);
}