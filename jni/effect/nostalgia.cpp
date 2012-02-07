#include "nostalgia.h"

static const char Nostalgia_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char Nostalgia_FShader[] = 
    "precision mediump float;\n"
    "uniform sampler2D uImgTex;\n"
    "varying vec2 vTexCoord;\n"
    "float mid(float a, float b, float c){\n"
    "    if((a-b)*(b-c)>0.0) return b;\n"
    "    else if((b-a)*(a-c)>0.0) return a;\n"
    "    else return c;\n"
    "}\n"
    "void main()\n"
    "{\n"
    "    vec3 color = texture2D( uImgTex, vTexCoord ).rgb;\n"
    "    float minrgb = min(min(color.r, color.g), color.b);\n"
    "    float maxrgb = max(max(color.r, color.g), color.b);\n"
    "    vec3 outcolor = color;\n"
    "    if (color.r < color.g && color.r < color.b){\n"
    "        float lim = mid(color.r, color.g, color.b) - minrgb;\n"
    "        outcolor.b *= (1.0 - lim);\n"
    "    }\n"
    "    if (color.r > 0.5 && color.g > 0.5 && color.b > 0.5){\n"
    "        float lim = (minrgb - 0.5)*2.0;\n"
    "        float dec = lim*color.b;\n"
    "        outcolor.b -= min(dec, lim*0.14);\n"
    "    }\n"
    "    if (color != vec3(0.0) && color != vec3(1.0)){\n"
    "        float lim = 1.0 - (abs(maxrgb - 0.5) + abs(minrgb - 0.5));\n"
    "        float dec = lim*color.b;\n"
    "        outcolor.b -= min(dec, lim*0.11);\n"
    "    }\n"
    "    if (color.r < 0.5 && color.g < 0.5 && color.b < 0.5){\n"
    "        float lim = (0.5-maxrgb)*2.0;\n"
    "        outcolor *= (1.0 - lim);\n"
    "    }\n"
    "    #define GammaCorrection(color, gamma)  pow(color, 1.0 / gamma)\n"
    "    #define LevelsControlInputRange(color, minInput, maxInput) min(max(color - vec3(minInput), vec3(0.0)) / (vec3(maxInput) - vec3(minInput)), vec3(1.0))\n"
    "    #define LevelsControlInput(color, minInput, gamma, maxInput) GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma)\n"
    "    outcolor = LevelsControlInput(outcolor, 16.0/255.0, vec3(0.96), 228.0/255.0);\n"
    "    if (outcolor.r > 0.5 && outcolor.g > 0.5 && outcolor.b > 0.5){\n"
    "        float lim = (min(min(outcolor.r, outcolor.g), outcolor.b) - 0.5)*2.0;\n"
    "        float dec = lim*outcolor.b;\n"
    "        outcolor.b -= min(dec, lim*0.08);\n"
    "    }\n"
    "    outcolor *= 1.16;\n"
    "    gl_FragColor = vec4(outcolor, 1.0);\n"
    "}\n";

Nostalgia::Nostalgia( void )
{
    mShader.makeProgram(Nostalgia_VShader, Nostalgia_FShader);
}

void Nostalgia::apply( Texture* input, Texture* output )
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

const char* Nostalgia::getName()
{
    return Nostalgia_Effect_Name;
}

void Nostalgia::setParameter( const char* parameterKey, float value )
{
    
}

float Nostalgia::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* Nostalgia::getParameterKeys()
{
    return "";
}
