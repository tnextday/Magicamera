#include "infrared.h"

static const char InfraRed_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char InfraRed_FShader[] = 
    "precision mediump float;\n"
    "uniform sampler2D uImgTex;\n"
    "varying vec2 vTexCoord;\n"
    "float getLuminance(vec3 color)\n"
    "{\n"
    "    float fmin = min(min(color.r, color.g), color.b);\n"
    "    float fmax = max(max(color.r, color.g), color.b);\n"
    "    return (fmax + fmin) / 2.0;\n"
    "}\n"
    "vec3 RGBToHSL(vec3 color)\n"
    "{\n"
    "    vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)\n"
    "    \n"
    "    float fmin = min(min(color.r, color.g), color.b);\n"
    "    float fmax = max(max(color.r, color.g), color.b);\n"
    "    float delta = fmax - fmin;\n"
    "    hsl.z = (fmax + fmin) / 2.0;\n"
    "\n"
    "    if (delta == 0.0)\n"
    "    {\n"
    "        hsl.x = 0.0;\n"
    "        hsl.y = 0.0;\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "        if (hsl.z < 0.5)\n"
    "            hsl.y = delta / (fmax + fmin);\n"
    "        else\n"
    "            hsl.y = delta / (2.0 - fmax - fmin); \n"
    "        \n"
    "        float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;\n"
    "        float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;\n"
    "        float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;\n"
    "        if (color.r == fmax )\n"
    "            hsl.x = deltaB - deltaG; \n"
    "        else if (color.g == fmax)\n"
    "            hsl.x = (1.0 / 3.0) + deltaR - deltaB; \n"
    "        else if (color.b == fmax)\n"
    "            hsl.x = (2.0 / 3.0) + deltaG - deltaR; \n"
    "        if (hsl.x < 0.0)\n"
    "            hsl.x += 1.0;\n"
    "        else if (hsl.x > 1.0)\n"
    "            hsl.x -= 1.0;\n"
    "    }\n"
    "    return hsl;\n"
    "}\n"
    "float HueToRGB(float f1, float f2, float hue)\n"
    "{\n"
    "    if (hue < 0.0)\n"
    "        hue += 1.0;\n"
    "    else if (hue > 1.0)\n"
    "        hue -= 1.0;\n"
    "    float res;\n"
    "    if ((6.0 * hue) < 1.0)\n"
    "        res = f1 + (f2 - f1) * 6.0 * hue;\n"
    "    else if ((2.0 * hue) < 1.0)\n"
    "        res = f2;\n"
    "    else if ((3.0 * hue) < 2.0)\n"
    "        res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;\n"
    "    else\n"
    "        res = f1;\n"
    "    return res;\n"
    "}\n"
    "vec3 HSLToRGB(vec3 hsl)\n"
    "{\n"
    "    vec3 rgb;\n"
    "    \n"
    "    if (hsl.y == 0.0)\n"
    "        rgb = vec3(hsl.z); // Luminance\n"
    "    else\n"
    "    {\n"
    "        float f2;\n"
    "        \n"
    "        if (hsl.z < 0.5)\n"
    "            f2 = hsl.z * (1.0 + hsl.y);\n"
    "        else\n"
    "            f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);\n"
    "            \n"
    "        float f1 = 2.0 * hsl.z - f2;\n"
    "        \n"
    "        rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));\n"
    "        rgb.g = HueToRGB(f1, f2, hsl.x);\n"
    "        rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));\n"
    "    }\n"
    "    \n"
    "    return rgb;\n"
    "}\n"
    "vec3 BlendLuminosity(vec3 base, vec3 blend)\n"
    "{\n"
    "    vec3 baseHSL = RGBToHSL(base);\n"
    "    return HSLToRGB(vec3(baseHSL.r, baseHSL.g, getLuminance(blend)));\n"
    "}\n"
    "void main()\n"
    "{\n"
    "    vec3 color = texture2D( uImgTex, vTexCoord ).rgb;\n"
    "    vec3 rColor = 1.0 - color;\n"
    "    vec3 oColor = BlendLuminosity(rColor, color);\n"
    "    gl_FragColor = vec4(oColor, 1.0);\n"
    "}";

InfraRed::InfraRed( void )
{
    mShader.makeProgram(InfraRed_VShader, InfraRed_FShader);
}

void InfraRed::apply( Texture* input, Texture* output )
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

const char* InfraRed::getName()
{
    return InfraRed_Effect_Name;
}

void InfraRed::setParameter( const char* parameterKey, float value )
{
    
}

float InfraRed::getParameterValue( const char* parameterKey )
{
    return 0;
}

const char* InfraRed::getParameterKeys()
{
    return "";
}
