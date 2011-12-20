#include "imageadjust.h"
#include <string.h>

static const char g_Adjust_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";

static const char g_Adjust_FShader[] = 
    "precision mediump float;\n"
    "varying vec2 vTexCoord;\n"
    "uniform sampler2D sTexture;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(sTexture, vTexCoord);\n"
    "}\n";

const int ADJU1 = 2;
const int ADJV1 = 3;
const int ADJU2 = 6;
const int ADJV2 = 7;
const int ADJU3 = 10;
const int ADJV3 = 11;
const int ADJU4 = 14;
const int ADJV4 = 15;

ImageAdjust::ImageAdjust(void)
{
    m_rorateCount = 0;
    m_bNeedAdjust = false;
    m_bXfliped = false;
    m_bYfliped = false;
    reset();
}

ImageAdjust::~ImageAdjust(void)
{
    SafeDelete(m_fbo);
}

void ImageAdjust::flip( bool x, bool y )
{
    if (x) {
        float m_u = m_QuadData[ADJU3];
        float m_u2 = m_QuadData[ADJU1];
        m_QuadData[ADJU1] = m_u;
        m_QuadData[ADJU2] = m_u;
        m_QuadData[ADJU3] = m_u2;
        m_QuadData[ADJU4] = m_u2;
    }
    if (y) {
        float m_v = m_QuadData[ADJV2];
        float m_v2 = m_QuadData[ADJV4];
        m_QuadData[ADJV1] = m_v2;
        m_QuadData[ADJV2] = m_v;
        m_QuadData[ADJV3] = m_v;
        m_QuadData[ADJV4] = m_v2;
    }
}

void ImageAdjust::rotate90( bool clockwise )
{

    if (clockwise) {
        float temp = m_QuadData[ADJV1];
        m_QuadData[ADJV1] = m_QuadData[ADJV4];
        m_QuadData[ADJV4] = m_QuadData[ADJV3];
        m_QuadData[ADJV3] = m_QuadData[ADJV2];
        m_QuadData[ADJV2] = temp;

        temp = m_QuadData[ADJU1];
        m_QuadData[ADJU1] = m_QuadData[ADJU4];
        m_QuadData[ADJU4] = m_QuadData[ADJU3];
        m_QuadData[ADJU3] = m_QuadData[ADJU2];
        m_QuadData[ADJU2] = temp;

        m_rorateCount++;
    } else {
        float temp = m_QuadData[ADJV1];
        m_QuadData[ADJV1] = m_QuadData[ADJV2];
        m_QuadData[ADJV2] = m_QuadData[ADJV3];
        m_QuadData[ADJV3] = m_QuadData[ADJV4];
        m_QuadData[ADJV4] = temp;

        temp = m_QuadData[ADJU1];
        m_QuadData[ADJU1] = m_QuadData[ADJU2];
        m_QuadData[ADJU2] = m_QuadData[ADJU3];
        m_QuadData[ADJU3] = m_QuadData[ADJU4];
        m_QuadData[ADJU4] = temp;
        m_rorateCount--;
    }
    m_bNeedAdjust = m_rorateCount%4 != 0;
}

void ImageAdjust::setSize(int w, int h)
{
    if (m_rorateCount%2 == 0){
        m_width = w;
        m_height = h;
    } else {
        m_width = h;
        m_height = w;
    }

}


bool ImageAdjust::init()
{
    m_shader.makeProgram(g_Adjust_VShader, g_Adjust_FShader);
    if (!m_shader.isCompiled()) {
        LOGE("Could not create program.\n");
        return false;
    }
    reset();
    m_fbo = new FramebufferObject();
    return true;
}


void ImageAdjust::reset()
{
    GLfloat Quad[] =
    {
        // X, Y, U, V
        -1.0f, -1.0f, 0.0f, 1.0f,
        +1.0f, -1.0f, 1.0f, 1.0f,
        +1.0f, +1.0f, 1.0f, 0.0f,
        -1.0f, +1.0f, 0.0f, 0.0f,
    };
    memcpy(m_QuadData, Quad, sizeof(GLfloat)*16);
}


bool ImageAdjust::apply( Texture* src, Texture* dst )
{
    m_shader.use();
    setSize(src->getWidth(), src->getHeight());
    dst->setSize(m_width, m_height);
    m_fbo->bindWithTexture(dst->getTexHandle());
    glViewport(0, 0, m_width, m_height);
/*    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);*/
    glClear(GL_COLOR_BUFFER_BIT);
    src->bind();
    //glEnableVertexAttribArray(m_shader.getPositionLoc());
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1); //话说不关掉这个就会报错 
    glVertexAttribPointer(m_shader.getPositionLoc(), 4, GL_FLOAT, GL_FALSE, 0, m_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    m_fbo->unbind();
    return true;
}