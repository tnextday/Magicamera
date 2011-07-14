#define _WIN32_WINNT 0x0502
#include <stdio.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>
#include "magicengine.h"
#include "main.h"


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
const CHAR*  g_strWindowTitle = "MagicAmera";
const UINT32 g_nWindowWidth   = 800;
const UINT32 g_nWindowHeight  = 480;
const bool g_useCamera = false;
const int g_cameraFPSRate = 18;
const int TIMER_UPDATE_NV21 = 1;
const char* g_strNV21Path = "f:\\nv21\\%03d.nv21";
const char* g_strSaveImagePath = "f:\\test.tga";
const char* g_resPath = "assets\\";
MagicEngine g_MagicEngine;
WinCallBack g_WinCallBack;


bool WinCallBack::SaveImage( char* buffer, int w, int h, int format )
{
    struct tgaheader_t
    {
        GLubyte   idLength;
        GLubyte   colorMapType;
        GLubyte   imageType;
        GLubyte   colorMapSpec[5];
        GLushort  xOrigin;
        GLushort  yOrigin;
        GLushort  width;
        GLushort  height;
        GLubyte   bpp;
        GLubyte   imageDesc;
    };
    struct rgb565_t{
        GLushort
        b : 5,
        g : 6,
        r : 5;
    };

#define TGA_RGB 2
    FILE* pFile;

    pFile = fopen(g_strSaveImagePath, "wb");
    if (!pFile)	return false;

    // read in the image type
    tgaheader_t tga;		// TGA header
    memset(&tga, 0, sizeof(tgaheader_t));
    tga.bpp = 32;
    tga.height = h;
    tga.width = w;
    tga.imageType = TGA_RGB;
    long szData = w*h*4;
    swapRedAndBlue((char*)buffer, w, h);
    fwrite(&tga, sizeof(tgaheader_t), 1, pFile);
    fwrite(buffer, sizeof(GLubyte), szData, pFile);
    if (pFile) fclose(pFile);
    return true;

}

void WinCallBack::swapRedAndBlue( char* buffer, int w, int h )
{
    struct rgba_t{
        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    };
    int total = w * h;
    GLubyte temp;
    rgba_t* source = (rgba_t*)buffer;
    for (int pixel = 0; pixel < total; ++pixel)
    {
        temp = source->b;
        source->b = source->r;
        source->r = temp;
        source++;
    }
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    POINTS vMousePt;
    switch( uMsg )
    {
    case WM_LBUTTONDOWN:
        vMousePt = MAKEPOINTS(lParam);
        g_MagicEngine.onTouchDown(vMousePt.x, vMousePt.y);
        return 0;
    case WM_MOUSEMOVE:
        if( wParam & MK_LBUTTON ) {
            vMousePt = MAKEPOINTS(lParam);
            g_MagicEngine.onTouchDrag(vMousePt.x, vMousePt.y);
        }
        return 0;
    case  WM_LBUTTONUP:
        vMousePt = MAKEPOINTS(lParam);
        g_MagicEngine.onTouchUp(vMousePt.x, vMousePt.y);
        return 0;
    case WM_TIMER:
        if (wParam == TIMER_UPDATE_NV21){
            updateNV21();
        }
        return 0;
    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;
    }

    // Pass all unhandled messages to the default WndProc
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


char* readFile(char* filename, int &size, char* preBuffer /*= NULL*/){
    TCHAR szPath[_MAX_PATH];
    if (filename[1] != ':'){
        GetCurrentDirectory(_MAX_PATH, szPath);
        sprintf(szPath, "%s\\%s", szPath, filename);
    }else{
        sprintf(szPath, "%s", filename);
    }
    FILE *fp = fopen(szPath, "rb");
    if (!fp) return NULL;
    char* buffer = NULL;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);            // determine file size so we can fill it in later if FileSize == 0
    if (size <= 0) goto __end ;
    fseek(fp, 0, SEEK_SET);
    if (preBuffer){
        buffer = preBuffer;
    }else{
        buffer = new char[size];
    }
    
    if (fread(buffer, sizeof(char), size, fp) <= 0){
        if (!preBuffer){
            delete[] buffer;
        }
        buffer = NULL;
    }

__end:
    fclose(fp);
    return buffer;
}

int szFile;
static char preBuffer[640*480*12/8];

void updateNV21(){
    static int idx = 0;
    static int step = 1;
    char path[_MAX_PATH];
__REREAD:
    _snprintf(path, _MAX_PATH-1, g_strNV21Path, idx);
    if (!readFile(path, szFile, preBuffer)){
        step = -step;
        idx += step;
        goto __REREAD;
    }
    g_MagicEngine.updatePreviewTex(preBuffer, szFile);
    idx += step;
}

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


int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    // Create the application window
    NativeWindowType hWindow;
    {
        // The global instance
        HINSTANCE hInstance  = GetModuleHandle( NULL );

        // Register the window class
        WNDCLASS wc = {0};
        wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Window style
        wc.lpfnWndProc    = (WNDPROC)WndProc;                    // WndProc message handler
        wc.hInstance      = hInstance;                           // Instance
        wc.lpszClassName  = "Adreno SDK Window";            // Set the class name
        wc.hCursor        = LoadCursor( NULL, IDC_ARROW );       // Cursor
        if( FALSE == RegisterClass(&wc) )
            return FALSE;

        // Adjust the window size to fit our rectangle
        DWORD dwWindowStyle = WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_BORDER |
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        RECT rcWindow;
        SetRect( &rcWindow, 0, 0, g_nWindowWidth, g_nWindowHeight );
        AdjustWindowRect( &rcWindow, dwWindowStyle, FALSE );

        // Create the parent window
        hWindow = CreateWindowEx(
            WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,    // Extended style
            "Adreno SDK Window",                   // Class
            g_strWindowTitle,                      // Title
            dwWindowStyle,                         // Style
            50 + rcWindow.left, 50 + rcWindow.top, // Position
            (rcWindow.right-rcWindow.left),        // Width
            (rcWindow.bottom-rcWindow.top),        // Height
            NULL, NULL, hInstance, NULL );
        if( NULL == hWindow )
            return FALSE;

        ShowWindow( hWindow, SW_SHOW );
        SetForegroundWindow( hWindow );
        SetFocus( hWindow );
    }

    // Get the display
    EGLDisplay eglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
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
            EGL_STENCIL_SIZE,   8,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_SAMPLES,        0,
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
    EGLSurface eglSurface = eglCreateWindowSurface( eglDisplay, eglConfig, hWindow, NULL );
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

    g_MagicEngine.setupGraphics(g_nWindowWidth, g_nWindowHeight);
    g_MagicEngine.setResPath(g_resPath);
    g_MagicEngine.setCallBack(&g_WinCallBack);
    g_MagicEngine.loadRes();

    if(g_useCamera){
        SetTimer(hWindow, TIMER_UPDATE_NV21, 1000/g_cameraFPSRate, NULL);
        g_MagicEngine.setPreviewDataInfo(640, 480, IMAGE_FORMAT_NV21);
    }else{
        int size;
        char* imgBuffer = NULL;
        imgBuffer = readFile("assets\\test.jpg", size);
        if (imgBuffer)
        {
            g_MagicEngine.setPreviewDataInfo(640, 480, IMAGE_FORMAT_PACKET);
            g_MagicEngine.updatePreviewTex(imgBuffer, size);
            delete[] imgBuffer;
        }
    }
    
    float lastTime = FrmGetTime();
    float timenow;
    // Run the main loop until the user closes the window
    while( TRUE )
    {
        MSG msg;
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
                return FALSE;
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }else{
            timenow = FrmGetTime();
            float delta = timenow - lastTime;
            lastTime = timenow;
            // Update and render the application
            g_MagicEngine.renderFrame(delta);

            // Present the scene
            eglSwapBuffers( eglDisplay, eglSurface );
        }
    }
    if(g_useCamera)
        KillTimer(hWindow, TIMER_UPDATE_NV21);

    return TRUE;
}


