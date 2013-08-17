#include "eglhelper.h"
#include <stdio.h>

//--------------------------------------------------------------------------------------
// Name: FrmGetTime()
// Desc: Platform-dependent function to get the current time (in seconds).
//--------------------------------------------------------------------------------------
float FrmGetTime()
{
    static BOOL     bInitialized = false;
    static LONGLONG m_llQPFTicksPerSec;
    static LONGLONG m_llBaseTime;
    if( false == bInitialized )
    {
        LARGE_INTEGER qwTicksPerSec;
        QueryPerformanceFrequency( &qwTicksPerSec );
        m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;

        LARGE_INTEGER qwTime;
        QueryPerformanceCounter( &qwTime );
        m_llBaseTime = qwTime.QuadPart;

        bInitialized = TRUE;
        return 0.0f;
    }

    // Get the current time
    LARGE_INTEGER qwTime;
    QueryPerformanceCounter( &qwTime );
    double fAppTime = (double)( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
    return (float)fAppTime;
}

int eglCreateSurface(HWND hWnd, EGLSurface &eglSurface, EGLDisplay &eglDisplay)
{
    HDC hDC		= 0;
    hDC = GetDC(hWnd);
    if (!hDC)
    {
        printf("Failed to create the device context");
        return FALSE;
    }

    eglDisplay = eglGetDisplay(hDC);
    // Get the display
    if(eglDisplay == EGL_NO_DISPLAY)
        eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

    if( eglDisplay == EGL_NO_DISPLAY )
        return FALSE;

    // Initialize EGL
    EGLint nMajorVersion, nMinorVersion;
    if( FALSE == eglInitialize( eglDisplay, &nMajorVersion, &nMinorVersion ) )
        return FALSE;

    // Set our EGL API to OpenGL ES
    if( FALSE == eglBindAPI( EGL_OPENGL_ES_API ) )
        return FALSE;

    // Get total number of configs
    EGLint nNumConfigs;
    if( FALSE == eglGetConfigs( eglDisplay, NULL, 0, &nNumConfigs ) )
        return FALSE;

    // Get the first config
    EGLConfig configs[10];
    if (FALSE == eglGetConfigs(eglDisplay, configs, 1, &nNumConfigs) )
        return FALSE;

    // Get the first config's Color buffer size
    EGLint nBuffSize;
    eglGetConfigAttrib(eglDisplay, configs[0], EGL_BUFFER_SIZE, &nBuffSize);

    // Test the display configuration
    EGLConfig eglConfig;

    if (nBuffSize == 32)
    {
        // Build the attibute list
        EGLint AttributeList[] = 
        { 
            EGL_RED_SIZE,        8,
            EGL_GREEN_SIZE,        8,
            EGL_BLUE_SIZE,        8,
            EGL_ALPHA_SIZE,        8,
            EGL_DEPTH_SIZE,        16,
            EGL_STENCIL_SIZE,   8,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_SAMPLES,        0,
            EGL_NONE
        };

        // Choose config based on the requested attributes
        if( FALSE == eglChooseConfig( eglDisplay, AttributeList, &eglConfig, 1, &nNumConfigs ) )
            return FALSE;
    }
    else if (nBuffSize == 24)
    {
        // Build the attibute list
        EGLint AttributeList[] = 
        { 
            EGL_RED_SIZE,        8,
            EGL_GREEN_SIZE,        8,
            EGL_BLUE_SIZE,        8,
            EGL_ALPHA_SIZE,        0,
            EGL_DEPTH_SIZE,        16,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
        };

        // Choose config based on the requested attributes
        if( FALSE == eglChooseConfig( eglDisplay, AttributeList, &eglConfig, 1, &nNumConfigs ) )
            return FALSE;
    }
    else if (nBuffSize == 16)
    {
        // Build the attibute list
        EGLint AttributeList[] = 
        { 
            EGL_RED_SIZE,        5,
            EGL_GREEN_SIZE,        6,
            EGL_BLUE_SIZE,        5,
            EGL_ALPHA_SIZE,        0,
            EGL_DEPTH_SIZE,        16,
            EGL_STENCIL_SIZE,   8,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_SAMPLES,        0,
            EGL_NONE
        };

        // Choose config based on the requested attributes
        if( FALSE == eglChooseConfig( eglDisplay, AttributeList, &eglConfig, 1, &nNumConfigs ) )
            return FALSE;
    }
    else return FALSE; // unsupported display

    // Create a window surface
    eglSurface = eglCreateWindowSurface( eglDisplay, eglConfig, hWnd, NULL );
    if( EGL_NO_SURFACE == eglSurface )
        return FALSE;

    // Create a OpenGL ES 2.0 rendering context
    EGLint ContextAttribList[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext eglContext = eglCreateContext( eglDisplay, eglConfig, EGL_NO_CONTEXT, ContextAttribList );
    if( EGL_NO_CONTEXT == eglContext )
        return FALSE;

    // Make the context current
    if( FALSE == eglMakeCurrent( eglDisplay, eglSurface, eglSurface, eglContext ) )
        return FALSE;
    return TRUE;
}