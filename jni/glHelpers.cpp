/**
 * Samsung Project
 * Copyright (c) 2008 Mobile Solution, Samsung Electronics, Inc.
 * All right reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung Electronics.
 */

/**
 * @file    glHelpers.cpp
 * @brief    3D driver's helper library
 * @author    Yue Li
 * @version    1.0
 * 
 * @history
 *   - add matrix function for gles2.0 application
 */

#include <math.h>
#include "fastmath.h"
#include <float.h>

#include "glHelpers.h"


/*****************************************************************************************/


void matIdentity(float m[16])
{
    for(int i=0; i<16; i++)
    {
        m[i] = (i%5==0)? 1.0f : 0.0f;  //The first and every fifth element after that is 1.0 other are 0.0
    }
}

/*
#ifndef fabs
float fabs(float f)
{
    return (f >0)? f : -f;
}
#endif
*/

bool matInverse(float inverse[16], const float src[16])
{
    float t;
    int i, j, k, swap;
    float tmp[4][4];
    
    matIdentity(inverse);
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tmp[i][j] = src[i*4+j];
        }
    }
    
    for (i = 0; i < 4; i++) {
        /* look for largest element in column. */
        swap = i;
        for (j = i + 1; j < 4; j++) {
            if (fabs(tmp[j][i]) > fabs(tmp[i][i])) {
                swap = j;
            }
        }
        
        if (swap != i) {
            /* swap rows. */
            for (k = 0; k < 4; k++) {
                t = tmp[i][k];
                tmp[i][k] = tmp[swap][k];
                tmp[swap][k] = t;
                
                t = inverse[i*4+k];
                inverse[i*4+k] = inverse[swap*4+k];
                inverse[swap*4+k] = t;
            }
        }
        
        if (tmp[i][i] == 0) {
        /* no non-zero pivot.  the matrix is singular, which
           shouldn't happen.  This means the user gave us a bad
            matrix. */
            return false;
        }
        
        t = tmp[i][i];
        for (k = 0; k < 4; k++) {
            tmp[i][k] /= t;
            inverse[i*4+k] /= t;
        }
        for (j = 0; j < 4; j++) {
            if (j != i) {
                t = tmp[j][i];
                for (k = 0; k < 4; k++) {
                    tmp[j][k] -= tmp[i][k]*t;
                    inverse[j*4+k] -= inverse[i*4+k]*t;
                }
            }
        }
    }
    return true;
}

/***************** Virdi added this set of code***********/
// m3=m1*m2
void matMultl(float* m3, const float* m1, const float* m2) 
{
        
       m3[0] = m2[0]*m1[0] + m2[4]*m1[1] + m2[8]*m1[2] + m2[12]*m1[3];
       m3[1] = m2[1]*m1[0] + m2[5]*m1[1] + m2[9]*m1[2] + m2[13]*m1[3];
       m3[2] = m2[2]*m1[0] + m2[6]*m1[1] + m2[10]*m1[2] + m2[14]*m1[3];
       m3[3] = m2[3]*m1[0] + m2[7]*m1[1] + m2[11]*m1[2] + m2[15]*m1[3];
       
       m3[4] = m2[0]*m1[4] + m2[4]*m1[5] + m2[8]*m1[6] + m2[12]*m1[7];
       m3[5] = m2[1]*m1[4] + m2[5]*m1[5] + m2[9]*m1[6] + m2[13]*m1[7];
       m3[6] = m2[2]*m1[4] + m2[6]*m1[5] + m2[10]*m1[6] + m2[14]*m1[7];
       m3[7] = m2[3]*m1[4] + m2[7]*m1[5] + m2[11]*m1[6] + m2[15]*m1[7];
       
       m3[8] = m2[0]*m1[8] + m2[4]*m1[9] + m2[8]*m1[10] + m2[12]*m1[11];
       m3[9] = m2[1]*m1[8] + m2[5]*m1[9] + m2[9]*m1[10] + m2[13]*m1[11];
       m3[10] = m2[2]*m1[8] + m2[6]*m1[9] + m2[10]*m1[10] + m2[14]*m1[11];
       m3[11] = m2[3]*m1[8] + m2[7]*m1[9] + m2[11]*m1[10] + m2[15]*m1[11];
       
       m3[12] = m2[0]*m1[12] + m2[4]*m1[13] + m2[8]*m1[14] + m2[12]*m1[15];
       m3[13] = m2[1]*m1[12] + m2[5]*m1[13] + m2[9]*m1[14] + m2[13]*m1[15];
       m3[14] = m2[2]*m1[12] + m2[6]*m1[13] + m2[10]*m1[14] + m2[14]*m1[15];
       m3[15] = m2[3]*m1[12] + m2[7]*m1[13] + m2[11]*m1[14] + m2[15]*m1[15];

}

void matMult(float* m3, const float* m1, const float* m2) 
{
        
       m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
       m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
       m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
       m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
       
       m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
       m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
       m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
       m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
       
       m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
       m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
       m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
       m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
       
       m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
       m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
       m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
       m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

}

//transform the vector x,y,z  with the transformation matrix m
void vecMult(const float* m, float x, float y, float z)
{    
    float xo,yo,zo;
       xo = m[0] * x + m[4] * y + m[8] * z + m[12];
       yo = m[1] * x + m[5] * y + m[9] * z + m[13];
       zo = m[2] * x + m[6] * y + m[10] * z + m[14];
       
       x=xo;
       y=yo;
       z=zo;
}

//will update the current matrix' m' with the tranalation vector x,y,z
void matTranslate(float* m, const float x, const float y, const float z  )
{
       m[12] = m[0] * x + m[4] *y + m[8]* z + m[12];
       m[13] = m[1] * x + m[5] *y + m[9]* z + m[13];
       m[14] = m[2] * x + m[6] *y + m[10]* z + m[14];
       m[15] = m[3] * x + m[7] *y + m[11]* z + m[15];
}  

//rotate about arbit axis
bool matRotate(float* m, float DEGAngle, float x, float y, float z)
{
        
       float Sq = (float) sqrt(x*x + y*y + z*z);
       float inv;
       if(Sq > -FLT_EPSILON && Sq < FLT_EPSILON) //chk for divide by zero......
           return false;
       inv = 1.0f/Sq;    
       x = x * inv;   
       y = y * inv;
       z = z * inv;
       float radian = (float)PI_OVER_180 * DEGAngle;
       float f32c = (float)fastcos(radian);
       float f32s = (float)fastsin(radian);
       float f32OneMinC = 1 - f32c;
        
        float RotMat[16];
       RotMat[0] = f32c + f32OneMinC * x * x;
       RotMat[1] = (f32OneMinC * x * y) + (z * f32s);
       RotMat[2] = (f32OneMinC * x * z) - (y * f32s);
       RotMat[3] = 0.0;
       RotMat[4] = (f32OneMinC * x * y) - (z * f32s);
       RotMat[5] = f32c + f32OneMinC * y * y;
       RotMat[6] = (f32OneMinC * y * z) + (x * f32s);
       RotMat[7] = 0.0;
       RotMat[8] = (f32OneMinC * x * z) + (y * f32s);
       RotMat[9] = (f32OneMinC * y * z) - (x * f32s);
       RotMat[10] = f32c + f32OneMinC * z * z;
       RotMat[11] = RotMat[12] =RotMat[13] = RotMat[14] = 0.0;RotMat[15] =1.0f;
       float t[16];
       matMult( &t[0] ,  m , &RotMat[0]);
       for(int i = 0; i < 16; ++i)
        m[i] = t[i];
        
      return true;
}

//will update the current matrix' m' with the tranalation vector x,y,z
void matTranslatel(float* m, const float x, const float y, const float z  )
{
       m[3] = m[0] * x + m[1] *y + m[2]* z + m[3];
       m[7] = m[4] * x + m[5] *y + m[6]* z + m[7];
       m[10] = m[8] * x + m[9] *y + m[10]* z + m[11];
       m[15] = m[12] * x + m[13] *y + m[14]* z + m[15];
}  

void matScale(float*m, float x, float y, float z)
{
    m[0] *= x ; m[4] *= y ; m[8] *= z ;// m[12] *= x ;  
    m[1] *= x ; m[5] *= y ; m[9] *= z ; //m[13] *= y ;  
    m[2] *= x ; m[6] *= y ; m[10] *= z ; //m[14] *= z ;  
}

//like glfrustum
bool matFrustum(float* m,float f32Left, float f32Right,float f32Bottom, float f32Top, float f32ZNear, float f32ZFar)
{
    float diff = f32Right - f32Left;
    if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
        return false;
    
    diff = f32Top - f32Bottom;
    if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
        return false;
        
    diff = f32ZFar - f32ZNear;
    if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
        return false;       
        
       m[0] = float(2.0*f32ZNear/(f32Right-f32Left));
       m[1] = m[2] = m[3] = 0;

       m[4] = 0;
       m[5] = float(2.0*f32ZNear/(f32Top-f32Bottom));
       m[6] = m[7] = 0;

       m[8] = (f32Right + f32Left) / (f32Right - f32Left);
       m[9] = (f32Top + f32Bottom) / (f32Top - f32Bottom);
       m[10] = -( (f32ZNear + f32ZFar) / (f32ZFar - f32ZNear)  );
       m[11] = -1;
       
       m[12] = m[13] =0;
       m[14] = -( (2*f32ZNear*f32ZFar) / (f32ZFar-f32ZNear));
       m[15] = 0;
       return true;
}

//like gluperspective matrix         
bool matPerspective(float* m ,float fieldOfViewDegree, float aspectRatio, float zNear, float zFar)
{
   if(fieldOfViewDegree <= 0.0f || fieldOfViewDegree >=180.0f)
      //fieldOfViewDegree = 45.0f;   //assign FOV to 45 deg if value passed is not in proper range 
       return false;

   float FOVrad = float(PI_OVER_180*fieldOfViewDegree*0.5f);//angle divided by 2 !!!

   float f32top = float( zNear*tan(FOVrad) );
   float f32Right = aspectRatio*f32top;
   return matFrustum(m,-f32Right,f32Right,-f32top,f32top,zNear,zFar);

}  

//like glortho
bool matOrtho(float* m,float f32Left, float f32Right,float f32Bottom, float f32Top, float f32ZNear, float f32ZFar)
{
    float diff = f32Right - f32Left;
    if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
        return false;
    
    diff = f32Top - f32Bottom;
    if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
        return false;
        
    diff = f32ZFar - f32ZNear;
    if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
        return false;
        
   m[0] = float(2.0/(f32Right-f32Left));
   m[1] = m[2] = m[3] = 0;

   m[4] = 0;
   m[5] = float(2.0/(f32Top-f32Bottom));
   m[6] = m[7] = 0;

   m[8] = m[9] = 0;
   m[10] = -float(2.0/(f32ZFar - f32ZNear));
   m[11] = 0;
   
   m[12] = -((f32Right+f32Left)/(f32Right-f32Left));
   m[13] = -((f32Top+f32Bottom)/(f32Top-f32Bottom));
   m[14] = -((f32ZNear+f32ZFar)/(f32ZFar-f32ZNear));
   m[15] = 1;
   return true;
}

bool vecNormalize(float& x, float& y, float& z)
{
       float Sq = (float) sqrt(x*x + y*y + z*z);
       float inv;
       if(Sq > -FLT_EPSILON && Sq < FLT_EPSILON) //chk for divide by zero......
           return false;
       inv = 1.0f/Sq;    
       x = x * inv;   
       y = y * inv;
       z = z * inv;
       return true;
}

//vector x = x1 cross y1
void vecCrossProduct(float& x,float& y, float& z , float x1,float y1, float z1 ,float x2,float y2, float z2)
{
    //x= y1*z2 - y1*z2;
    x= y1*z2 - y2*z1;
    y= z1*x2 - z2*x1;
    z= x1*y2 - x2*y1;
}         

//like gluLookAt
void matLookAt(float* m, float px, float py, float pz,float tx, float ty, float tz, float ux, float uy, float uz)
 {
    float sx,sy,sz;
    float uux,uuy,uuz;

    float zx = tx-px;
    float zy = ty-py;
    float zz = tz-pz;

    vecNormalize(zx,zy,zz);
    vecNormalize(ux,uy,uz);

    vecCrossProduct(sx,sy,sz,zx,zy,zz,ux,uy,uz);

    vecCrossProduct(uux,uuy,uuz,sx,sy,sz,zx,zy,zz);

    vecNormalize(sx,sy,sz);
    vecNormalize(uux,uuy,uuz);

    m[0]=sx; m[4]=sy; m[8]=sz; m[12]=0;
    m[1]=uux; m[5]=uuy; m[9]=uuz; m[13]=0;
    m[2]=-zx; m[6]=-zy; m[10]=-zz; m[14]=0;
    m[3]=0; m[7]=0; m[11]=0; m[15]=1.0;

    matTranslate(m,-px,-py,-pz);
 }

// liyue 080922
/*
void matPrint(float* m)
{
    for(int i=0; i<16; i++)
    {
        if(i%4 == 0) printf("\n");
        printf("%f  ",m[i]);
    }
    printf("\n");
}
*/
#define MOVESTEP    5.0
//rotate about arbit axis (reserve original matrix)
int matRotate_res(float* o, float* i, float DEGAngle, float x, float y, float z)
{

    static float inx = 0.0;
    static float iny = 0.0;
    static bool bxInc = true;
    static bool    byInc = true;

    float Sq = (float) sqrt(x*x + y*y + z*z);
    float inv;
    if(Sq > -FLT_EPSILON && Sq < FLT_EPSILON) //chk for divide by zero......
        return false;
    inv = 1.0f/Sq;    
    x = x * inv;   
    y = y * inv;
    z = z * inv;
    float radian = (float)PI_OVER_180 * 0;
    float f32c = (float)fastcos(radian);
    float f32s = (float)fastsin(radian);
    float f32OneMinC = 1 - f32c;

    float RotMat[16];
    RotMat[0] = f32c + f32OneMinC * x * x;
    RotMat[1] = (f32OneMinC * x * y) + (z * f32s);
    RotMat[2] = (f32OneMinC * x * z) - (y * f32s);
    RotMat[3] = 0.0;
    RotMat[4] = (f32OneMinC * x * y) - (z * f32s);
    RotMat[5] = f32c + f32OneMinC * y * y;
    RotMat[6] = (f32OneMinC * y * z) + (x * f32s);
    RotMat[7] = 0.0;
    RotMat[8] = (f32OneMinC * x * z) + (y * f32s);
    RotMat[9] = (f32OneMinC * y * z) - (x * f32s);
    RotMat[10] = f32c + f32OneMinC * z * z;
    RotMat[11] = RotMat[12] =RotMat[13] = RotMat[14] = 0.0;RotMat[15] =1.0f;

    matMult(o, i, RotMat);
    matTranslate(o, x + (inx),y + iny,z);
    if(bxInc)
    {
        inx += MOVESTEP;
    }
    else
    {
        inx -= MOVESTEP;
    }
    
    if(inx > 240)
    {
        bxInc = false;
    }

    if(inx < -240)
    {
        bxInc = true;
    }

    if(byInc)
    {
        iny += MOVESTEP;
    }
    else
    {
        iny -= MOVESTEP;
    }

    if(iny > 110)
    {
        byInc = false;
    }

    if(iny < -135)
    {
        byInc = true;
    }

    return true;
}

