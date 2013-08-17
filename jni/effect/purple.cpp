#include "purple.h"

static const char Purple_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char Purple_FShader[] = 
    "precision mediump float;\n"
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
    "#define GammaCorrection(color, gamma)                                pow(color, vec3(1.0 / gamma))  \n"
    "#define LevelsControlInputRange(color, minInput, maxInput)                min(max(color - vec3(minInput), vec3(0.0)) / (vec3(maxInput) - vec3(minInput)), vec3(1.0))\n"
    "#define LevelsControlInput(color, minInput, gamma, maxInput)                GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma)\n"
    "#define LevelsControlOutputRange(color, minOutput, maxOutput)             mix(vec3(minOutput), vec3(maxOutput), color)\n"
    "#define LevelsControl(color, minInput, gamma, maxInput, minOutput, maxOutput)     LevelsControlOutputRange(LevelsControlInput(color, minInput, gamma, maxInput), minOutput, maxOutput)\n"
    "void main()\n"
    "{\n"
    "    vec3 color = texture2D( uImgTex, vTexCoord ).rgb;\n"
    "    color.r = LevelsControl(color.r, 23.0/255.0, 0.8, 1.0, 8.0/255.0, 1.0).r;\n"
    "    color.b = LevelsControl(color.b, 0.0, 1.27, 1.0, 16.0/255.0, 220.0/255.0).r;\n"
    "    color =  LevelsControl(color, 7.0/255.0, 0.84, 252.0/255.0, 22.0/255.0, 1.0);\n"
    "    color.r = texture2D(uCurve, vec2(color.r, 0.5)).r;\n"
    "    color.g = texture2D(uCurve, vec2(color.g, 0.5)).g;\n"
    "    color.b = texture2D(uCurve, vec2(color.b, 0.5)).b;\n"
    "    vec3 outColor = ContrastSaturationBrightness(color, 1.02, 1.0, 1.05);\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}";

Purple::Purple( void )
{
    mShader.makeProgram(Purple_VShader, Purple_FShader);
    mColorCurve.loadFromFile("res://maps/PurpleCurve.png");
    mColorCurve.setFilter(GL_NEAREST, GL_NEAREST);
    mSrcTexLoc = mShader.getUniformLoc("uImgTex");
    mCurveTexLoc = mShader.getUniformLoc("uCurve");
}

void Purple::apply( Texture* input, Texture* output )
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

const char* Purple::getName()
{
    return Purple_Effect_Name;
}

void Purple::setParameter( const char* parameterKey, float value )
{
    
}

float Purple::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* Purple::getParameterKeys()
{
    return "";
}
