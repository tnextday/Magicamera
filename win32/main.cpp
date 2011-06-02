//--------------------------------------------------------------------------------------
// File: main.cpp
//
// Desc: Tutorial to show how to render a simple textured triangle using OpenGL ES 2.0.
//
// Author:     QUALCOMM, Advanced Content Group - Adreno SDK
//
//               Copyright (c) 2009 QUALCOMM Incorporated. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#define _WIN32_WINNT 0x0502
#include <stdio.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
const CHAR*  g_strWindowTitle = "SimpleTexture";
const UINT32 g_nWindowWidth   = 320;
const UINT32 g_nWindowHeight  = 240;
const FLOAT  g_fAspectRatio   = (FLOAT)g_nWindowWidth / (FLOAT)g_nWindowHeight;
const CHAR*  g_strImageFileName = "TreeBark.tga";

CHAR         g_strImageFilePath[512] = "";
GLuint       g_hTextureHandle = 0;
GLuint       g_hShaderProgram = 0;
GLuint       g_VertexLoc      = 0;
GLuint       g_TexcoordLoc    = 1;

//--------------------------------------------------------------------------------------
// Name: g_strVSProgram / g_strFSProgram
// Desc: The vertex and fragment shader programs
//--------------------------------------------------------------------------------------
const CHAR* g_strVSProgram = 
    "attribute vec4 g_vVertex;											 \n"
    "attribute vec4 g_vTexcoord;							 		     \n"
    "                                                                    \n"
    "varying   vec4 g_vVSTexcoord;                                       \n"
    "																	 \n"
    "void main()														 \n"
    "{																	 \n"
    "    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
    "                         g_vVertex.z, g_vVertex.w );                \n"
    "    g_vVSTexcoord = g_vTexcoord;                                    \n"
    "}																	 \n";

const CHAR* g_strFSProgram = 
    "#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
    "   precision highp float;											 \n"
    "#else																 \n"
    "   precision mediump float;										 \n"
    "#endif																 \n"
    "																	 \n"
    "uniform   sampler2D g_sImageTexture;                                \n"
    "varying   vec4      g_vVSTexcoord;                                  \n"
    "																	 \n"
    "void main()														 \n"
    "{																	 \n"
    "    gl_FragColor = texture2D(g_sImageTexture, g_vVSTexcoord.xy);    \n"
    "}																	 \n";

//--------------------------------------------------------------------------------------
// Name: LoadTGA()
// Desc: Helper function to load a 32-bit TGA image file
//--------------------------------------------------------------------------------------
BYTE* LoadTGA( const CHAR* strFileName, UINT32* pWidth, UINT32* pHeight, UINT32* nFormat )
{
    struct TARGA_HEADER
    {
        BYTE   IDLength, ColormapType, ImageType;
        BYTE   ColormapSpecification[5];
        UINT16 XOrigin, YOrigin;
        UINT16 ImageWidth, ImageHeight;
        BYTE   PixelDepth;
        BYTE   ImageDescriptor;
    };
    
    static TARGA_HEADER header;
    
    // Read the TGA file
    FILE* file = fopen( strFileName, "rb" );
    if( NULL == file )
        return NULL;

    fread( &header, sizeof(header), 1, file );
    UINT32 nPixelSize = header.PixelDepth / 8;
    (*pWidth)  = header.ImageWidth;
    (*pHeight) = header.ImageHeight;
    (*nFormat) = nPixelSize == 3 ? GL_RGB : GL_RGBA;

    BYTE* pBits = new BYTE[ nPixelSize * header.ImageWidth * header.ImageHeight ];
    fread( pBits, nPixelSize, header.ImageWidth * header.ImageHeight, file );
    fclose( file );

    // Convert the image from BGRA to RGBA
    BYTE* p = pBits;
    for( UINT32 y=0; y<header.ImageHeight; y++ )
    {
        for( UINT32 x=0; x<header.ImageWidth; x++ )
        {
            BYTE temp = p[2]; p[2] = p[0]; p[0] = temp;
            p += nPixelSize;
        }
    }

    return pBits;
}

//--------------------------------------------------------------------------------------
// Name: Initialize()
// Desc: Initialize the scene
//--------------------------------------------------------------------------------------
BOOL Initialize()
{
    // Create a texture, loaded from an image file
    {
        UINT32 nWidth, nHeight, nFormat;
        BYTE*  pImageData = LoadTGA( g_strImageFilePath, &nWidth, &nHeight, &nFormat );
        if( NULL == pImageData )
            return FALSE;

        glGenTextures( 1, &g_hTextureHandle );
        glBindTexture( GL_TEXTURE_2D, g_hTextureHandle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, nFormat, nWidth, nHeight,
                      0, nFormat, GL_UNSIGNED_BYTE, pImageData );
        delete[] pImageData;
    }

    // Create the shader program needed to render the texture
    {
        // Compile the shaders
        GLuint hVertexShader = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( hVertexShader, 1, &g_strVSProgram, NULL );
        glCompileShader( hVertexShader );

        GLuint hFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( hFragmentShader, 1, &g_strFSProgram, NULL );
        glCompileShader( hFragmentShader );

        // Check for compile success
        GLint nCompileResult = 0;
	    glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &nCompileResult);
	    if (!nCompileResult)
	    {
		    CHAR Log[1024];
		    GLint nLength;
		    glGetShaderInfoLog(hFragmentShader, 1024, &nLength, Log);
		    return FALSE;
	    }

        // Attach the individual shaders to the common shader program
        g_hShaderProgram = glCreateProgram();
        glAttachShader( g_hShaderProgram, hVertexShader );
        glAttachShader( g_hShaderProgram, hFragmentShader );

        // Init attributes BEFORE linking
        glBindAttribLocation(g_hShaderProgram, g_VertexLoc,   "g_vVertex");
        glBindAttribLocation(g_hShaderProgram, g_TexcoordLoc, "g_vTexcoord");

        // Link the vertex shader and fragment shader together
        glLinkProgram( g_hShaderProgram );

        // Check for link success
        GLint nLinkResult = 0;
	    glGetProgramiv(g_hShaderProgram, GL_LINK_STATUS, &nLinkResult);
	    if (!nLinkResult)
	    {
		    CHAR Log[1024];
		    GLint nLength;
		    glGetProgramInfoLog(g_hShaderProgram, 1024, &nLength, Log);
		    return FALSE;
	    }

        glDeleteShader( hVertexShader );
        glDeleteShader( hFragmentShader );
    }

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Cleanup any objects created in Initialize();
//--------------------------------------------------------------------------------------
VOID Cleanup()
{
    glDeleteTextures( 1, &g_hTextureHandle );
    glDeleteProgram( g_hShaderProgram );
}


//--------------------------------------------------------------------------------------
// Name: Render()
// Desc: Render the scene
//--------------------------------------------------------------------------------------
VOID Render()
{
    FLOAT fSize = 0.5f;
    FLOAT VertexPositions[] =
    {
         0.0f,  +fSize*g_fAspectRatio, 0.0f, 1.0f,
        -fSize, -fSize*g_fAspectRatio, 0.0f, 1.0f,
        +fSize, -fSize*g_fAspectRatio, 0.0f, 1.0f,
    };
	FLOAT VertexTexcoord[] = {0.5f, 1.0f,    
                              0.0f, 0.0f,    
                              1.0f, 0.0f    
                             };

    // Clear the colorbuffer and depth-buffer
    glClearColor( 0.0f, 0.0f, 0.5f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Set the shader program and the texture
    glUseProgram( g_hShaderProgram );
    glBindTexture( GL_TEXTURE_2D, g_hTextureHandle );

    // Draw the textured triangle
    glVertexAttribPointer( g_VertexLoc, 4, GL_FLOAT, 0, 0, VertexPositions );
    glEnableVertexAttribArray( g_VertexLoc );

	glVertexAttribPointer( g_TexcoordLoc, 2, GL_FLOAT, 0, 0, VertexTexcoord);
	glEnableVertexAttribArray( g_TexcoordLoc );

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );

    glDisableVertexAttribArray( g_VertexLoc );
    glDisableVertexAttribArray( g_TexcoordLoc );
}


//--------------------------------------------------------------------------------------
// Name: WndProc()
// Desc: The application window's message proc
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_CLOSE:
            PostQuitMessage( 0 );
            return 0;
    }

    // Pass all unhandled messages to the default WndProc
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


//--------------------------------------------------------------------------------------
// Name: WinMain()
// Desc: Application entry-point on the Windows platform
//--------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    CHAR* strSdkRootDir = getenv( "ADRENO_SDK_DIR" );
    if( strSdkRootDir )
    {
        // Set the DLL path so that the necessary DLLs can be found
        CHAR strDllPath[512] = "";
        strcat( strDllPath, strSdkRootDir );
        strcat( strDllPath, "\\System" );
        SetDllDirectory( strDllPath );

        // Set the data file path so that necessary image files can be found
        strcat( g_strImageFilePath, strSdkRootDir );
        strcat( g_strImageFilePath, "\\Tutorials\\OpenGLES\\08_SimpleTexture\\" );
        strcat( g_strImageFilePath, g_strImageFileName );
    }

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
        wc.lpszClassName  = "Adreno SDK Window";                 // Set the class name
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

    // Set our EGL API to OpenGL ES
    if( FALSE == eglBindAPI( EGL_OPENGL_ES_API ) )
        return FALSE;

    // Initialize EGL
    EGLint nMajorVersion, nMinorVersion;
    if( FALSE == eglInitialize( eglDisplay, &nMajorVersion, &nMinorVersion ) )
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
            EGL_RED_SIZE,		8,
            EGL_GREEN_SIZE,	    8,
            EGL_BLUE_SIZE,	    8,
            EGL_ALPHA_SIZE,	    8,
            EGL_DEPTH_SIZE,	    24,
            EGL_STENCIL_SIZE,   8,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_SAMPLES,		0,
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
            EGL_RED_SIZE,		5,
            EGL_GREEN_SIZE,	    6,
            EGL_BLUE_SIZE,	    5,
            EGL_ALPHA_SIZE,	    0,
            EGL_DEPTH_SIZE,	    24,
            EGL_STENCIL_SIZE,   8,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_SAMPLES,		0,
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

    if( FALSE == Initialize() )
        return FALSE;

    // Run the main loop until the user closes the window
    while( TRUE )
    {
        MSG msg;
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
                return FALSE;
        }

        TranslateMessage( &msg );
        DispatchMessage( &msg );

        // Update and render the application
        Render();

        // Present the scene
        eglSwapBuffers( eglDisplay, eglSurface );
    }

    // Cleanup any objects created in Initialize()
    Cleanup();

    return TRUE;
}

