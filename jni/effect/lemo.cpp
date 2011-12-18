#include "lemo.h"

static const char LEMO_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char LEMO_FShader[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "   precision highp float;\n"
    "#else\n"
    "   precision mediump float;\n"
    "#endif\n"
    "\n"
    "uniform sampler2D uImgTex;\n"
    "uniform sampler2D uCurve;\n"
    "varying vec2 vTexCoord;\n"
    "vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)\n"
    "{\n"
    "    const vec3 AvgLumin = vec3(0.5, 0.5, 0.5);\n"
    "    const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);\n"
    "    \n"
    "    vec3 brtColor = color * brt;\n"
    "    vec3 intensity = vec3(dot(brtColor, LumCoeff));\n"
    "    vec3 satColor = mix(intensity, brtColor, sat);\n"
    "    vec3 conColor = mix(AvgLumin, satColor, con);\n"
    "    return conColor;\n"
    "}\n"
    "void main()\n"
    "{\n"
    "    vec3 color = texture2D( uImgTex, vTexCoord ).rgb;\n"
    "    vec3 outColor = ContrastSaturationBrightness(color, 1.0, 1.2, 1.15);\n"
    "    color.r = texture2D(uCurve, vec2(outColor.r, 0.5)).r;\n"
    "    color.g = texture2D(uCurve, vec2(outColor.g, 0.5)).g;\n"
    "    color.b = texture2D(uCurve, vec2(outColor.b, 0.5)).b;\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}";

LEMO::LEMO( void )
{
    mShader.makeProgram(LEMO_VShader, LEMO_FShader);
    mColorCurve.loadFromFile("res://maps/LemoCurve.png");
    mColorCurve.setFilter(GL_NEAREST, GL_NEAREST);
    mSrcTexLoc = mShader.getUniformLoc("uImgTex");
    mCurveTexLoc = mShader.getUniformLoc("uCurve");
}

void LEMO::apply( Texture* input, Texture* output )
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
    glUniform1i(mCurveTexLoc, 1);
    mColorCurve.bind(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    mFBO.unbind();
}

const char* LEMO::getName()
{
    return LEMO_Effect_Name;
}

void LEMO::setParameter( const char* parameterKey, float value )
{
    
}

float LEMO::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* LEMO::getParameterKeys()
{
    return "";
}
