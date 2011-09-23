#include "render.h"
#include <QDir>

const char* g_testImagePath = "..\\test\\test.jpg";
const char* g_strNV21Path = "test\\nv21\\%03d.nv21";
const char* g_strSaveImagePath = "test.tga";
const char* g_resPath = "..\\assets\\";

char* readFile(char* filename, uint32_t &size, char* preBuffer /*= NULL*/){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return NULL;

    char* buffer = NULL;
    size = file.size();
    if (preBuffer){
        buffer = preBuffer;
    }else{
        buffer = new char[size];
    }
    if (file.read(buffer, size) <= 0){
        if (!preBuffer){
            delete[] buffer;
        }
        buffer = NULL;
    }

__end:
    file.close();
    return buffer;
}

bool WinCallBack::SaveImage( char* buffer, int w, int h, int format )
{
    bool result = false;
    QImage img(w, h, QImage::Format_RGB888);
    if (img.loadFromData((const uchar*)buffer, w*h*4)){
        img.rgbSwapped();
        result = img.save(g_strSaveImagePath, "JPG");
    }
    return result;

}


unsigned char* WinCallBack::readRes( const char* resname, uint32_t &size )
{
    char path[_MAX_PATH];
    _snprintf(path, _MAX_PATH-1, "%s\\%s", g_resPath, resname);
    return (unsigned char*)readFile(path, (uint32_t)size);
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

int eglCreateSurface(HWND hWnd, EGLSurface &eglSurface, EGLDisplay &eglDisplay)
{
    HDC hDC		= 0;
    hDC = GetDC(hWnd);
    if (!hDC)
    {
        LOGI(0, "Failed to create the device context", "Error", MB_OK|MB_ICONEXCLAMATION);
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


Render::Render(QWidget *parent)
    : QWidget(parent)
{
    this->resize(640, 480);
    if (!eglCreateSurface(this->winId(), m_eglSurface, m_eglDisplay))
        return;

    m_MagicMain.setupGraphics();
    m_MagicMain.setIOCallBack(&m_WinCallBack);
    m_MagicMain.setPreviewImage(g_testImagePath);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_renerTimer()));
    m_timer.setSingleShot(false);
    m_timer.start(1000/60);
    m_fpsTime = 0;
    m_lastTime = 0;
}

Render::~Render()
{

}

void Render::on_renerTimer()
{
    float timenow;
    timenow = FrmGetTime();
    float delta = timenow - m_lastTime;
    m_lastTime = timenow;
    // Update and render the application
    m_MagicMain.renderFrame(delta);
    // Present the scene
    eglSwapBuffers( m_eglDisplay, m_eglSurface );
    m_fpsFrames++;
    m_fpsTime += delta;
    if (m_fpsTime > 1){
        this->setWindowTitle(QString("Render - %1").arg(m_fpsFrames));
        m_fpsFrames = 0;
        m_fpsTime = 0;
    }
}

void Render::resizeEvent( QResizeEvent * e)
{
    m_MagicMain.resize(e->size().width(), e->size().height());
}

void Render::mouseMoveEvent( QMouseEvent * e)
{
    m_MagicMain.onTouchDrag(e->x(), e->y());
}

void Render::mouseReleaseEvent( QMouseEvent * e)
{
    m_MagicMain.onTouchUp(e->x(), e->y());
}

void Render::mousePressEvent( QMouseEvent * e)
{
    m_MagicMain.onTouchDown(e->x(), e->y());
}
