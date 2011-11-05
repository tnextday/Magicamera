#include "glow.h"
#include <string.h> 

static const char Glow_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char Glow_FShader_hblur[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "precision highp float;\n"
    "#else\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D RTScene;\n"
    "varying vec2 vTexCoord;\n"
    "uniform float hBlurSize;\n"
    "void main(void)\n"
    "{\n"
    "    vec4 sum = vec4(0.0);\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - 4.0*hBlurSize, vTexCoord.y)) * 0.05;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - 3.0*hBlurSize, vTexCoord.y)) * 0.09;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - 2.0*hBlurSize, vTexCoord.y)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - hBlurSize, vTexCoord.y)) * 0.15;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y)) * 0.16;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + hBlurSize, vTexCoord.y)) * 0.15;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + 2.0*hBlurSize, vTexCoord.y)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + 3.0*hBlurSize, vTexCoord.y)) * 0.09;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + 4.0*hBlurSize, vTexCoord.y)) * 0.05;\n"
    "    gl_FragColor = sum;\n"
    "}\n";
static const char Glow_FShader_vblur_final[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "precision highp float;\n"
    "#else\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D RTScene;\n"
    "uniform sampler2D SrcTex;\n"
    "varying vec2 vTexCoord;\n"
    "uniform float vBlurSize;\n"
    "uniform float Exposure;\n"
    "void main(void)\n"
    "{\n"
    "    vec4 sum = vec4(0.0);\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - 4.0*vBlurSize)) * 0.05;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - 3.0*vBlurSize)) * 0.09;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - 2.0*vBlurSize)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - vBlurSize)) * 0.15;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y)) * 0.16;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + vBlurSize)) * 0.15;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + 2.0*vBlurSize)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + 3.0*vBlurSize)) * 0.09;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + 4.0*vBlurSize)) * 0.05;\n"
    "    vec4 srcColor  = texture2D(SrcTex,vTexCoord);\n"
    "    #define BlendOverlayf(base, blend)    (base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))\n"
    "    #define Blend(base, blend, funcf)     vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b))\n"
    "    #define BlendOverlay(base, blend)     Blend(base, blend, BlendOverlayf)\n"
    "    #define BlendHardLight(base, blend)   BlendOverlay(blend, base)\n"
    "    gl_FragColor =  vec4(BlendHardLight(srcColor.rgb, sum.rgb), 1.0);\n"
    //"    gl_FragColor = min(srcColor, sum);\n"
    "}\n";


Glow::Glow(void)
{
    mBlurStep = 1;
    mHBlur.makeProgram(Glow_VShader, Glow_FShader_hblur);
    mHBlurSizeLoc = mHBlur.getUniformLoc("hBlurSize");
    mFinal.makeProgram(Glow_VShader, Glow_FShader_vblur_final);
    mVBlurSizeLoc = mFinal.getUniformLoc("vBlurSize");
    mExposureLoc = mFinal.getUniformLoc("Exposure");
    mRTSenceLoc = mFinal.getUniformLoc("RTScene");
    mSrcTexLoc = mFinal.getUniformLoc("SrcTex");
    mExposure = 0.5;
    mTmpTex.init();
}

Glow::~Glow(void)
{
}

void Glow::apply( Texture* input, Texture* output )
{
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    //横向模糊,渲染到临时纹理上 
    mHBlur.use();
    mHBlur.setAttrf(mHBlurSizeLoc, mBlurStep/output->getWidth());
    mTmpTex.setSize(output->getWidth(), output->getHeight());
    mFBO.bindWithTexture(mTmpTex.getTexHandle());
    glViewport(0, 0, output->getWidth(), output->getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    input->bind(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //纵向模糊，并且和原图片混合  
    mFinal.use();
    mFinal.setAttrf(mVBlurSizeLoc, mBlurStep/output->getHeight());
    mFinal.setAttrf(mExposureLoc, mExposure);
    mFBO.bindWithTexture(output->getTexHandle());
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1i(mSrcTexLoc, 0);
    input->bind(0);
    glUniform1i(mRTSenceLoc, 1);
    mTmpTex.bind(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    mFBO.unbind();
}

const char* Glow::getName()
{
    return Glow_Effect_Name;
}

void Glow::setParameter( const char* parameterKey, float value )
{
    if (!parameterKey) return;
    if (strcmp("exposure", parameterKey) == 0){
        mExposure = value;
    } else if (strcmp("blurstep", parameterKey) == 0){
        mBlurStep = value;
    }
}

float Glow::getParameterValue( const char* parameterKey )
{
    if (!parameterKey) return 0;
    if (strcmp("exposure", parameterKey) == 0){
        return mExposure;
    } else if (strcmp("blurstep", parameterKey) == 0){
        return mBlurStep;
    }
    return 0;
}

const char* Glow::getParameterKeys()
{
    return "[{key:'blurstep'; type:'float'; max:5.0; min:0.0;}]";
}