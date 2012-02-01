#include "Reversal.h"

static const char Reversal_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char Reversal_FShader[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "   precision highp float;\n"
    "#else\n" 
    "   precision mediump float;\n"
    "#endif\n" 
    "uniform sampler2D uImgTex;\n"
    "varying vec2 vTexCoord;\n"
    "#define BlendColorBurnf(base, blend)  ((blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0))\n"
    "#define GammaCorrection(color, gamma)  pow(color, 1.0 / gamma)\n"
    "#define LevelsControlInputRange(color, minInput, maxInput) min(max(color - minInput, 0.0) / (maxInput - minInput), 1.0)\n"
    "#define LevelsControlInput(color, minInput, gamma, maxInput) GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma)\n"
    "vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)\n"
    "{\n"
    "    const vec3 AvgLumin = vec3(0.5, 0.5, 0.5);\n"
    "    const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);\n"
    "    vec3 brtColor = color * brt;\n"
    "    vec3 intensity = vec3(dot(brtColor, LumCoeff));\n"
    "    vec3 satColor = mix(intensity, brtColor, sat);\n" 
    "    vec3 conColor = mix(AvgLumin, satColor, con);\n"
    "    return conColor;\n"
    "}\n"
    "void main()\n" 
    "{\n"
    "    vec3 vSample = texture2D( uImgTex, vTexCoord ).rgb;\n"
    "    vec3 color;\n"
    "    color.b = vSample.b*(1.0-vSample.b*0.5);\n"
    "    color.g = vSample.g*(1.0-vSample.g*0.2);\n"
    "    color.r = BlendColorBurnf(vSample.r, vSample.r);\n"
    "    color.b = LevelsControlInput(color.b, 25.0/255.0, 0.75, 150.0/255.0);\n"
    "    color.g = LevelsControlInput(color.g, 40.0/255.0, 1.20, 220.0/255.0);\n"
    "    color.r = LevelsControlInput(color.r, 50.0/255.0, 1.30, 255.0/255.0);\n"
    "    vec3 color2 = ContrastSaturationBrightness(color, 0.95, 1.15, 1.20);\n"
    "    gl_FragColor = vec4(color2, 1.0);\n"
    "}";

Reversal::Reversal( void )
{
    mShader.makeProgram(Reversal_VShader, Reversal_FShader);
}

void Reversal::apply( Texture* input, Texture* output )
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

const char* Reversal::getName()
{
    return Reversal_Effect_Name;
}

void Reversal::setParameter( const char* parameterKey, float value )
{
    
}

float Reversal::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* Reversal::getParameterKeys()
{
    return "";
}
