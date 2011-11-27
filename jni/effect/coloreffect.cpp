#include "coloreffect.h"

static const char ColorEffect_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char ColorEffect_FShader[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n" 
    "   precision highp float;\n" 
    "#else\n"
    "   precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D uImgTex;\n"
    "uniform sampler2D uLookUp;\n"
    "varying vec2 vTexCoord;\n"
    "void main()\n"
    "{\n"
    "    vec3 vSample = texture2D( uImgTex, vTexCoord ).rgb;\n"
    "    float r = vSample.r*127.0;\n"
    "    float u = (floor(mod(r, 8.0))*128.0 + vSample.g*127.0)/1024.0;\n"
    "    float v = (floor(r/8.0)*128.0 + vSample.b*127.0)/2048.0;\n"
    "    vec3 color = texture2D(uLookUp, vec2(u, v)).rgb; \n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

ColorEffect::ColorEffect( void )
{
    mShader.makeProgram(ColorEffect_VShader, ColorEffect_FShader);
    mColorIndex.loadFromFile("res://rgb777.png");
    mColorIndex.setFilter(GL_NEAREST, GL_NEAREST);
    mSrcTexLoc = mShader.getUniformLoc("uImgTex");
    mLookupTexLoc = mShader.getUniformLoc("uLookUp");
}

void ColorEffect::apply( Texture* input, Texture* output )
{
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    mShader.use();
    mFBO.bindWithTexture(output->getTexHandle());
    glViewport(0, 0, output->getWidth(), output->getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1i(mSrcTexLoc, 0);
    input->bind(0);
    glUniform1i(mLookupTexLoc, 1);
    mColorIndex.bind(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    mFBO.unbind();
}

const char* ColorEffect::getName()
{
    return ColorEffect_Effect_Name;
}

void ColorEffect::setParameter( const char* parameterKey, float value )
{
    
}

float ColorEffect::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* ColorEffect::getParameterKeys()
{
    return "";
}
