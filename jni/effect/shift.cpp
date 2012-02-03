#include "shift.h"
#include <string.h> 

static const char Shift_VShader[] = 
    "attribute vec4 aPosition;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition.x, -aPosition.y, 0.0, 1.0);\n" 
    "    vTexCoord = vec2(aPosition.z, aPosition.w );\n"
    "}\n";
static const char Shift_FShader_hblur[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "precision highp float;\n"
    "#else\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D RTScene;\n"
    "varying vec2 vTexCoord;\n"
    "uniform float hBlurSize;\n"
    "uniform float edge0;\n"
    "uniform float edge1;\n"
    "vec4 blurH(){\n"
    "   vec4 sum = vec4(0.0);\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x - 2.0*hBlurSize, vTexCoord.y)) * 1.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x - hBlurSize, vTexCoord.y)) * 2.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y)) * 3.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x + hBlurSize, vTexCoord.y)) * 2.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x + 2.0*hBlurSize, vTexCoord.y)) * 1.0/9.0;\n"
    "   return sum;\n"
    "}"
    "void main(void)\n"
    "{\n"
    "   if (vTexCoord.y > edge0 || vTexCoord.y < edge1){\n"
    "       gl_FragColor = blurH();\n"
    "   }else{\n"
    "       gl_FragColor = vec4(0.0);\n"
    "   }\n"
    "}\n";
static const char Shift_FShader_vblur_final[] = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
    "precision highp float;\n"
    "#else\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D RTScene;\n"
    "uniform sampler2D SrcTex;\n"
    "varying vec2 vTexCoord;\n"
    "uniform float vBlurSize;\n"
    "uniform float edge0;\n"
    "uniform float edge1;\n"
    "vec4 blurV(){\n"
    "   vec4 sum = vec4(0.0);\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - 2.0*vBlurSize)) * 1.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y - vBlurSize)) * 2.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y)) * 3.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + vBlurSize)) * 2.0/9.0;\n"
    "   sum += texture2D(RTScene, vec2(vTexCoord.x, vTexCoord.y + 2.0*vBlurSize)) * 1.0/9.0;\n"
    "   return sum;\n"
    "}\n"
    "void main(void)\n"
    "{\n"
    "   if (vTexCoord.y > edge0 || vTexCoord.y < edge1){\n"
    "       gl_FragColor = blurV();\n"
    "   }else{\n"
    "       gl_FragColor = texture2D(SrcTex, vTexCoord);\n"
    "   }\n"
    "}\n";


Shift::Shift(void)
{
    mBlurStep = 1;
    mEdge0 = 0.75;
    mEdge1 = 0.25;
    mHBlur.makeProgram(Shift_VShader, Shift_FShader_hblur);
    mHBlurSizeLoc = mHBlur.getUniformLoc("hBlurSize");
    mEdge00Loc = mHBlur.getUniformLoc("edge0");
    mEdge01Loc = mHBlur.getUniformLoc("edge1");

    mFinal.makeProgram(Shift_VShader, Shift_FShader_vblur_final);
    mVBlurSizeLoc = mFinal.getUniformLoc("vBlurSize");
    mRTSenceLoc = mFinal.getUniformLoc("RTScene");
    mSrcTexLoc = mFinal.getUniformLoc("SrcTex");
    mEdge10Loc = mFinal.getUniformLoc("edge0");
    mEdge11Loc = mFinal.getUniformLoc("edge1");
    mTmpTex.init();
}

Shift::~Shift(void)
{
}

void Shift::apply( Texture* input, Texture* output )
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
    glUniform1f(mEdge00Loc, mEdge0-0.05);
    glUniform1f(mEdge01Loc, mEdge1+0.05);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //纵向模糊 
    mFinal.use();
    mFinal.setAttrf(mVBlurSizeLoc, mBlurStep/output->getHeight());
    mFBO.bindWithTexture(output->getTexHandle());
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(mEdge10Loc, mEdge0);
    glUniform1f(mEdge11Loc, mEdge1);
    glUniform1i(mSrcTexLoc, 0);
    input->bind(0);
    glUniform1i(mRTSenceLoc, 1);
    mTmpTex.bind(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, G_QuadData);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    mFBO.unbind();
}

const char* Shift::getName()
{
    return Shift_Effect_Name;
}

void Shift::setParameter( const char* parameterKey, float value )
{
    if (!parameterKey) return;
    if (strcmp("blurstep", parameterKey) == 0){
        mBlurStep = value;
    }else if (strcmp("edge0", parameterKey) == 0){
        mEdge0 = value;
    }else if (strcmp("edge1", parameterKey) == 0){
        mEdge1 = value;
    }
}

float Shift::getParameterValue( const char* parameterKey )
{
    if (!parameterKey) return 0;
    if (strcmp("blurstep", parameterKey) == 0){
        return mBlurStep;
    }else if (strcmp("edge0", parameterKey) == 0){
        return mEdge0;
    }else if (strcmp("edge1", parameterKey) == 0){
        return mEdge1;
    }
    return 0;
}

const char* Shift::getParameterKeys()
{
    return "["
        "{key:'blurstep'; type:'float'; max:5.0; min:0.0;},"
        "{key:'edge0'; type:'float'; max:1.0; min:0.0;},"
        "{key:'edge1'; type:'float'; max:1.0; min:0.0;}"
        "]";
}