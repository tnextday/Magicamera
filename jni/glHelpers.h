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
 * @file	glHelpers.h
 * @brief	3D driver's helper library
 * @author	Yue Li
 * @version	1.0
 * 
 * @history
 *   - add matrix function for gles2.0 application
 */


#ifndef __GLHELPERS_H__
#define __GLHELPERS_H__

#define PI (3.141592)
#define PI_OVER_180 (PI/180.0)


#ifdef __cplusplus
extern "C" {
#endif
void matTranspose(float* m);
void matIdentity(float m[16]);
bool matInverse(float inverse[16], const float src[16]);
void matMult(float* m3, const float* m1, const float* m2) ;
void matTranslate(float* m, const float x, const float y, const float z  );
bool matRotate(float* m, float DEGAngle, float x, float y, float z);
void matScale(float*m, float x, float y, float z);
bool matFrustum(float* m,float f32Left, float f32Right,float f32Bottom, float f32Top, float f32ZNear, float f32ZFar);
bool matPerspective(float* m ,float fieldOfViewDegree, float aspectRatio, float zNear, float zFar);
bool matOrtho(float* m,float f32Left, float f32Right,float f32Bottom, float f32Top, float f32ZNear, float f32ZFar);
void vecMult(const float* m, float x, float y, float z);

void matLookAt(float* m, float px, float py, float pz,float tx, float ty, float tz, float ux, float uy, float uz);
int  matRotate_res(float* o, float* i, float DEGAngle, float x, float y, float z);

#ifdef __cplusplus
}
#endif


#endif //__GLHELPERS_H__
