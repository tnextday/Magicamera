#include "scanline.h"

static const char ScanLine_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char ScanLine_FShader[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n" 
    "   precision highp float;\n" 
    "#else\n"
    "   precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D uImgTex;\n"
    "varying vec2 vTexCoord;\n"
    "void main(void)\n"
    "{\n"
    "    vec3 oricol = texture2D(uImgTex,vTexCoord).xyz;\n"
    "    vec3 col;\n"
    "    col.r = texture2D(uImgTex,vec2(vTexCoord.x+0.003, vTexCoord.y)).x;\n"
    "    col.g = texture2D(uImgTex,vec2(vTexCoord.x      , vTexCoord.y)).y;\n"
    "    col.b = texture2D(uImgTex,vec2(vTexCoord.x-0.003, vTexCoord.y)).z;\n"
    "    col = clamp(col*0.5+0.5*col*col*1.2, 0.0, 1.0);\n"
    "    col *= 0.5 + 0.5*16.0*vTexCoord.x*vTexCoord.y*(1.0-vTexCoord.x)*(1.0-vTexCoord.y);\n"
    "    col *= vec3(0.8,1.0,0.7);\n"
    "    col *= 0.9+0.1*sin(vTexCoord.y*1000.0);\n"
    "    col = mix( col, oricol, clamp(-2.0+2.0*vTexCoord.x,0.0,1.0) );\n"
    "    gl_FragColor = vec4(col,1.0);\n"
    "}\n";

ScanLine::ScanLine( void )
{
    mShader.makeProgram(ScanLine_VShader, ScanLine_FShader);
}

void ScanLine::apply( Texture* input, Texture* output )
{
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    mShader.use();
    mFBO.bindWithTexture(output->getTexHandle());
    glViewport(0, 0, output->getWidth(), output->getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    input->bind(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    mFBO.unbind();
}

const char* ScanLine::getName()
{
    return ScanLine_Effect_Name;
}

void ScanLine::setParameter( const char* parameterKey, float value )
{
    
}

float ScanLine::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* ScanLine::getParameterKeys()
{
    return "";
}
