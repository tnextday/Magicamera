#define _WIN32_WINNT 0x0502
//#include <vld.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <EGL/egl.h>
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>
#include "magic/magicmain.h"
#include "main.h"
#include <time.h>


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

const CHAR*  g_strWindowTitle = "MagicAmera";
const UINT32 g_nglWinWidth   = 480;
const UINT32 g_nglWinHeight  = 640;
const UINT32 g_nToolBarHeight = 32;
const UINT32 g_nWindowWidth   = 480;
const UINT32 g_nWindowHeight  = g_nglWinHeight + g_nToolBarHeight;
const UINT32 g_PicWidth = 640;
const UINT32 g_PicHeigth = 480;
const bool g_useCamera = true;
const int g_cameraFPSRate = 18;
const int TIMER_UPDATE_NV21 = 1;
const char* g_testImagePath = "assets\\test3.jpg";
const char* g_strNV21Path = ".\\nv21\\%03d.nv21";
const char* g_strSaveImagePath = "test.tga";
const char* g_resPath = "assets\\";
MagicMain g_MagicMain;
WinCallBack g_WinCallBack;

EGLSurface g_eglSurface;
EGLDisplay g_eglDisplay;
HWND hbtn_save;
HWND hbtn_change_engine;
HWND hbtn_change_effect;
HWND hbtn_change_func;
HWND hbtn_rotate_left;
HWND hbtn_rotate_right;


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

unsigned char* WinCallBack::readRes( const char* resname, uint32_t &size )
{
     char path[_MAX_PATH];
     _snprintf(path, _MAX_PATH-1, "%s\\%s", g_resPath, resname);
     return (unsigned char*)readFile(path, (uint32_t)size);
}


int eglCreateSurface(HWND hWin, EGLSurface &eglSurface, EGLDisplay &eglDisplay)
{
    // Get the display
    eglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
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
    eglSurface = eglCreateWindowSurface( eglDisplay, eglConfig, hWin, NULL );
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

char* readFile(char* filename, uint32_t &size, char* preBuffer /*= NULL*/){
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

uint32_t szFile;
static char preBuffer[g_PicWidth*g_PicHeigth*12/8];

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
    g_MagicMain.updatePreviewData(preBuffer, szFile);
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

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    POINTS vMousePt;
    switch( uMsg )
    {
    case WM_LBUTTONDOWN:
        vMousePt = MAKEPOINTS(lParam);
        g_MagicMain.onTouchDown(vMousePt.x, vMousePt.y);
        return 0;
    case WM_MOUSEMOVE:
        if( wParam & MK_LBUTTON ) {
            vMousePt = MAKEPOINTS(lParam);
            g_MagicMain.onTouchDrag(vMousePt.x, vMousePt.y);
        }
        return 0;
    case  WM_LBUTTONUP:
        vMousePt = MAKEPOINTS(lParam);
        g_MagicMain.onTouchUp(vMousePt.x, vMousePt.y);
        return 0;
    case WM_TIMER:
        if (wParam == TIMER_UPDATE_NV21){
            updateNV21();
        }
        return 0;
    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;
    case WM_COMMAND:
        if (lParam == (int)hbtn_save){
            g_MagicMain.takePicture();
        } else if(lParam == (int)hbtn_change_effect){

        } else if(lParam == (int)hbtn_change_engine){
            EngineType type;
            if (g_MagicMain.getEngineType() == EngineType_Mesh){
                SetWindowText(hbtn_change_func, "切换相框");
                type = EngineType_Cover;
            } else {
                SetWindowText(hbtn_change_func, "复  原");
                type = EngineType_Mesh;
            }
            g_MagicMain.switchEngine(type);
        } else if(lParam == (int)hbtn_change_func){
            if (g_MagicMain.getEngineType() == EngineType_Mesh){
                g_MagicMain.restoreMesh();
            }else{
                srand(time(0));
                char path[_MAX_PATH];
                _snprintf(path, _MAX_PATH-1, "%s\\frames/%.2d.png", g_resPath, rand()%18);
                uint32_t size;
                unsigned char* buffer = (unsigned char*)readFile(path, size);
                g_MagicMain.setCover(buffer, size);
                delete [] buffer;
            }
        } else if(lParam == (int)hbtn_rotate_left){
            g_MagicMain.rotate90Input(false);
        } else if(lParam == (int)hbtn_rotate_right){
            g_MagicMain.rotate90Input(true);
        }
        return 0;
    }

    // Pass all unhandled messages to the default WndProc
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{

    // Create the application window
    HWND hGlWin;
    NativeWindowType hWindow;
    {
        // The global instance
        HINSTANCE hInstance  = GetModuleHandle( NULL );

        // Register the window class
        WNDCLASS wc = {0};
        wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Window style
        wc.lpfnWndProc    = (WNDPROC)WndProc;                    // WndProc message handler
        wc.hInstance      = hInstance;                           // Instance
        wc.lpszClassName  = "MagicWindow";                       // Set the class name
        wc.hbrBackground  = (HBRUSH)COLOR_WINDOW;                //背景颜色 
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
            "MagicWindow",                   // Class
            g_strWindowTitle,                      // Title
            dwWindowStyle,                         // Style
            50 + rcWindow.left, 50 + rcWindow.top, // Position
            (rcWindow.right-rcWindow.left),        // Width
            (rcWindow.bottom-rcWindow.top),        // Height
            NULL, NULL, hInstance, NULL );
        if( NULL == hWindow )
            return FALSE;

        hGlWin = CreateWindowEx(0,"Static","",WS_CHILD | WS_VISIBLE,
            0,0, g_nglWinWidth,g_nglWinHeight,
            hWindow ,0,hInstance, NULL);

        HWND hToolBar = CreateWindowEx(0,"MagicWindow","",WS_CHILD | WS_VISIBLE,
            0,g_nglWinHeight,g_nWindowWidth,g_nToolBarHeight,
            hWindow ,0,hInstance, NULL);

        int btnTop = 2;
        int btnWidth = 60;
        int btnHeigth = 30;
        int btnSpace = 5;
        int left = 1;
        hbtn_save = CreateWindowEx(0,"Button","保 存",WS_CHILD | WS_VISIBLE,
            left,btnTop,btnWidth,btnHeigth,
            hToolBar ,0,hInstance, NULL);
        left += btnWidth + btnSpace;
        hbtn_change_effect = CreateWindowEx(0,"Button","切换特效",WS_CHILD | WS_VISIBLE,
            left,btnTop,btnWidth,btnHeigth,
            hToolBar ,0,hInstance, NULL);
        left += btnWidth + btnSpace;
        hbtn_change_engine = CreateWindowEx(0,"Button","切换引擎",WS_CHILD | WS_VISIBLE,
            left,btnTop,btnWidth,btnHeigth,
            hToolBar ,0,hInstance, NULL);
        left += btnWidth + btnSpace;
        hbtn_change_func = CreateWindowEx(0,"Button","复  原",WS_CHILD | WS_VISIBLE,
            left,btnTop,btnWidth,btnHeigth,
            hToolBar ,0,hInstance, NULL);

        btnWidth = 40;
        left = g_nWindowWidth - 41;
        hbtn_rotate_right = CreateWindowEx(0,"Button",">>",WS_CHILD | WS_VISIBLE,
            left,btnTop,btnWidth,btnHeigth,
            hToolBar ,0,hInstance, NULL);
        left -= btnWidth + btnSpace;
        hbtn_rotate_left = CreateWindowEx(0,"Button","<<",WS_CHILD | WS_VISIBLE,
            left,btnTop,btnWidth,btnHeigth,
            hToolBar ,0,hInstance, NULL);
        //--创建字体--
        HFONT MyFont_Hanlde = CreateFont(-14, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET,
                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

        //--设置字体--
        SendMessage(hbtn_save , WM_SETFONT, (WPARAM)MyFont_Hanlde, 0);
        SendMessage(hbtn_change_effect, WM_SETFONT, (WPARAM)MyFont_Hanlde, 0);
        SendMessage(hbtn_change_engine, WM_SETFONT, (WPARAM)MyFont_Hanlde, 0);
        SendMessage(hbtn_change_func, WM_SETFONT, (WPARAM)MyFont_Hanlde, 0);


        ShowWindow( hWindow, SW_SHOW );
        SetForegroundWindow( hWindow );
        SetFocus( hWindow );
    }

    if (!eglCreateSurface(hGlWin, g_eglSurface, g_eglDisplay))
        return FALSE;

    g_MagicMain.setupGraphics(g_nglWinWidth, g_nglWinHeight);
    g_MagicMain.setIOCallBack(&g_WinCallBack);

    if(g_useCamera){
        SetTimer(hWindow, TIMER_UPDATE_NV21, 1000/g_cameraFPSRate, NULL);
        g_MagicMain.setPreviewDataInfo(g_PicWidth, g_PicHeigth, IMAGE_FORMAT_NV21);
        g_MagicMain.rotate90Input();
    }else{
        g_MagicMain.setPreviewImage(g_testImagePath);
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
            g_MagicMain.renderFrame(delta);

            // Present the scene
            eglSwapBuffers( g_eglDisplay, g_eglSurface );
        }
    }
    if(g_useCamera)
        KillTimer(hWindow, TIMER_UPDATE_NV21);

    return TRUE;
}


