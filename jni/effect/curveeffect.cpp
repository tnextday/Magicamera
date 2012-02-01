#include "curveeffect.h"

static const char CurveEffect_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char CurveEffect_FShader[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "   precision highp float;\n"
    "#else\n"
    "   precision mediump float;\n"
    "#endif\n"
    "\n"
    "uniform sampler2D uImgTex;\n"
    "uniform sampler2D uCurve;\n"
    "varying vec2 vTexCoord;\n"
    "void main()\n"
    "{\n"
    "    vec3 color = texture2D( uImgTex, vTexCoord ).rgb;\n"
    "    color.r = texture2D(uCurve, vec2(color.r, 0.5)).r;\n"
    "    color.g = texture2D(uCurve, vec2(color.g, 0.5)).g;\n"
    "    color.b = texture2D(uCurve, vec2(color.b, 0.5)).b;\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}";

CurveEffect::CurveEffect( void )
{
    mShader.makeProgram(CurveEffect_VShader, CurveEffect_FShader);
    mSrcTexLoc = mShader.getUniformLoc("uImgTex");
    mCurveTexLoc = mShader.getUniformLoc("uCurve");
}

void CurveEffect::apply( Texture* input, Texture* output )
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


void CurveEffect::setParameter( const char* parameterKey, float value )
{
    
}

float CurveEffect::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* CurveEffect::getParameterKeys()
{
    return "";
}

void CurveEffect::setCurveTex( const char* texPath )
{
    mColorCurve.loadFromFile(texPath);
    mColorCurve.setFilter(GL_NEAREST, GL_NEAREST);
}