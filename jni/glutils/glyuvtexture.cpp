#include "glyuvtexture.h"
#include <string.h>

/*
* ʹ��GL shader ������YUV420SP2RGBת��������GPUǿ���ĸ������㣬ת���ٶ�ΪCPU��3������
* ����YUV420SP�Ĵ���ṹ��������w*h�ֽڵ�y���ݣ�Ȼ��Ϊw*h/2�ֽڵ�uv�������У�ÿ2��y����һ��UV
* ʹ������Texture�������ݸ�GPU��һ������Y���ݣ���һ��ר��UV����
* Y����ʹ��LUMINANCE���ݣ�ÿ������ռһ���ֽڣ�
* UVʹ��GL_LUMINANCE_ALPHA���ݣ�ÿ������ռ�����ֽڣ���shader��ʹ��rgb.ra��ʹ������
* ��������ֱ��ʹ��GPU����YUV420SP�����ؽ����ڴ濽��
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

glYUVTexture::glYUVTexture(int w, int h, GLuint texid)
{
    m_fbo = NULL;
    init(w, h, texid);
}

glYUVTexture::~glYUVTexture(void)
{
    glDeleteTextures(2, m_YUVTexs);
    SafeDelete(m_fbo);
}

bool glYUVTexture::init( int w, int h, GLuint texid )
{

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

    m_width = w;
    m_height = h;
    m_fbo = new FramebufferObject();
    m_fbo->texture2d(texid);
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

void glYUVTexture::uploadYUVTexImage( char* yuv420sp, int w, int h )
{
    if (w != m_width || h != m_height) {
        LOGE("uploadYUVTexImage Error 0\n");
        return;
    }

    m_shader.use();
    glDisable(GL_DEPTH_TEST);
    m_fbo->bind();
    //printGLColorSpaceInfo();
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_YUVTexs[YTexId_idx]);
    glUniform1i(m_uYtexLoc, 1);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,w,h,0,GL_LUMINANCE,GL_UNSIGNED_BYTE, yuv420sp);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_YUVTexs[UVTexId_idx]);
    glUniform1i(m_uUVTexLoc, 2);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,w/2,h/2,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE, yuv420sp+w*h);

    
    //glEnableVertexAttribArray(m_shader.getPositionLoc());
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1); //��˵���ص�����ͻᱨ��
    glVertexAttribPointer(m_shader.getPositionLoc(), 4, GL_FLOAT, GL_FALSE, 0, QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_fbo->unbind();
    checkGlError("uploadYUVTexImage");
}

void glYUVTexture::setTargetTexId( GLuint texid )
{
    m_fbo->texture2d(texid);
}
