#pragma once
#include <EGL/egl.h>
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>

float FrmGetTime();
int eglCreateSurface(HWND hWnd, EGLSurface &eglSurface, EGLDisplay &eglDisplay);