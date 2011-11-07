#include "hdr.h"
#include <string.h> 

static const char HDR_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char HDR_FShader_hblur[] = 
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
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - 4.0*hBlurSize, vTexCoord.y)) * 0.04;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - 3.0*hBlurSize, vTexCoord.y)) * 0.08;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - 2.0*hBlurSize, vTexCoord.y)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x - hBlurSize, vTexCoord.y)) * 0.16;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y)) * 0.20;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + hBlurSize, vTexCoord.y)) * 0.16;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + 2.0*hBlurSize, vTexCoord.y)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + 3.0*hBlurSize, vTexCoord.y)) * 0.08;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x + 4.0*hBlurSize, vTexCoord.y)) * 0.04;\n"
    "    gl_FragColor = sum;\n"
    "}\n";

static const char HDR_FShader_vblur[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "precision highp float;\n"
    "#else\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D RTScene;\n"
    "varying vec2 vTexCoord;\n"
    "uniform float vBlurSize;\n"
    "void main(void)\n"
    "{\n"
    "    vec4 sum = vec4(0.0);\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - 4.0*vBlurSize)) * 0.04;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - 3.0*vBlurSize)) * 0.08;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - 2.0*vBlurSize)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - vBlurSize)) * 0.16;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y)) * 0.20;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + vBlurSize)) * 0.16;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + 2.0*vBlurSize)) * 0.12;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + 3.0*vBlurSize)) * 0.08;\n"
    "    sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + 4.0*vBlurSize)) * 0.04;\n"
    "    gl_FragColor = sum;\n"
    "}\n";

static const char HDR_FShader_final[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "precision highp float;\n"
    "#else\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D SrcTex;\n"
    "uniform sampler2D LightTex;\n"
    "uniform float Exposure;\n"
    "varying vec2 vTexCoord;\n"
    "vec4 xposure(vec4 _color , float gray , float ex)\n"
    "{\n"
    "    float b = (4.0 * ex - 1.0);\n"
    "    float a = 1.0 - b;\n"
    "    float f = gray * ( a * gray + b );\n"
    "    return f * _color;\n"
    "}\n"
    "void main(void)\n"
    "{\n"
    "vec4 color  = texture2D(SrcTex, vTexCoord);\n"
    "vec4 lightColor = texture2D(LightTex, vTexCoord);\n"
    "float lum = 0.3 * lightColor.x + 0.59 * lightColor.y + 0.11* lightColor.z;\n"
    "gl_FragColor =  xposure(color , lum , Exposure);\n"
    "}\n";

HDR::HDR(void)
{
    mBlurStep = 1;
    mHBlur.makeProgram(HDR_VShader, HDR_FShader_hblur);
    mHBlurSizeLoc = mHBlur.getUniformLoc("hBlurSize");
    mVBlur.makeProgram(HDR_VShader, HDR_FShader_vblur);
    mVBlurSizeLoc = mVBlur.getUniformLoc("vBlurSize");

    mFinal.makeProgram(HDR_VShader, HDR_FShader_final);
    mExposureLoc = mFinal.getUniformLoc("Exposure");
    mLightTexLoc = mFinal.getUniformLoc("LightTex");
    mSrcTexLoc = mFinal.getUniformLoc("SrcTex");
    mExposure = 1.0;
    mTmpTex.init();
    mTmpTex2.init();
}

HDR::~HDR(void)
{
}

void HDR::apply( Texture* input, Texture* output )
{
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    //横向模糊,渲染到临时纹理上,并且缩放为原来的1/4大小
    int sw, sh;
    sw = output->getWidth()/4;
    sh = output->getHeight()/4;
    mHBlur.use();
    mHBlur.setAttrf(mHBlurSizeLoc, mBlurStep/output->getWidth());
    mTmpTex.setSize(sw, sh);
    mFBO.bindWithTexture(mTmpTex.getTexHandle());
    glViewport(0, 0, sw, sh);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    input->bind(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //纵向模糊
    mVBlur.use();
    mVBlur.setAttrf(mVBlurSizeLoc, mBlurStep/output->getHeight());
    mTmpTex2.setSize(sw, sh);
    mFBO.bindWithTexture(mTmpTex2.getTexHandle());
    glViewport(0, 0, sw, sh);
    glClear(GL_COLOR_BUFFER_BIT);
    mTmpTex.bind(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //调整亮度并且和原图片混合
    mFinal.use();
    mFinal.setAttrf(mExposureLoc, mExposure);
    mFBO.bindWithTexture(output->getTexHandle());
    glViewport(0, 0, output->getWidth(), output->getHeight());
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1i(mSrcTexLoc, 0);
    input->bind(0);
    glUniform1i(mLightTexLoc, 1);
    mTmpTex2.bind(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    mFBO.unbind();
}

const char* HDR::getName()
{
    return HDR_Effect_Name;
}

void HDR::setParameter( const char* parameterKey, float value )
{
    if (!parameterKey) return;
    if (strcmp("exposure", parameterKey) == 0){
        mExposure = value;
    } else if (strcmp("blurstep", parameterKey) == 0){
        mBlurStep = value;
    }
}

float HDR::getParameterValue( const char* parameterKey )
{
    if (!parameterKey) return 0;
    if (strcmp("exposure", parameterKey) == 0){
        return mExposure;
    } else if (strcmp("blurstep", parameterKey) == 0){
        return mBlurStep;
    }
    return 0;
}

const char* HDR::getParameterKeys()
{
    return "["
        "{key:'exposure'; max:2.0; min:0.5;},"
        "{key:'blurstep'; max:5.0; min:0.0;}"
        "]";
}