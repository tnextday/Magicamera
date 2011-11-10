#include "Overlay.h"

static const char Overlay_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char Overlay_FShader[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n" 
    "   precision highp float;\n" 
    "#else\n"
    "   precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D uImgTex;\n"
    "varying vec2 vTexCoord;\n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = texture2D(uImgTex,vTexCoord);\n"
    "}\n";

Overlay::Overlay( void )
{
    mShader.makeProgram(Overlay_VShader, Overlay_FShader);
    m_sCoverFactor = GL_ONE;
    m_dCoverFactor = GL_ONE;
}

void Overlay::apply( Texture* input, Texture* output )
{
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    mShader.use();
    mFBO.bindWithTexture(output->getTexHandle());
    glViewport(0, 0, output->getWidth(), output->getHeight());
    input->bind(0);
    glEnable(GL_BLEND);
    glBlendFunc(m_sCoverFactor, m_dCoverFactor);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    mFBO.unbind();
}

const char* Overlay::getName()
{
    return Overlay_Effect_Name;
}

void Overlay::setParameter( const char* parameterKey, float value )
{
    
}

float Overlay::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* Overlay::getParameterKeys()
{
    return "";
}

void Overlay::setBlendFunc( GLenum sfactor, GLenum dfactor )
{
    m_sCoverFactor = sfactor;
    m_dCoverFactor = dfactor;
}