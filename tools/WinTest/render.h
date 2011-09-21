#ifndef RENDER_H
#define RENDER_H
#include <QWidget>
#include <EGL/egl.h>
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>
#include "magic/magicmain.h"
#include <QTimer>
#include <QResizeEvent>

char* readFile(char* filename, uint32_t &size, char* preBuffer = NULL);
float FrmGetTime();
int eglCreateSurface(HWND hWnd, EGLSurface &eglSurface, EGLDisplay &eglDisplay);

class WinCallBack: public FileUtils{
public:
    virtual bool SaveImage( char* buffer, int w, int h, int format );
    virtual unsigned char* readRes( const char* resname, uint32_t& size );
};


class Render : public QWidget
{
    Q_OBJECT
private:
    float m_lastTime;
    float m_fpsTime;
    int     m_fpsFrames;
public:
    MagicMain       g_MagicMain;
    WinCallBack     g_WinCallBack;
    EGLSurface      g_eglSurface;
    EGLDisplay      g_eglDisplay;

    QTimer          m_timer;
public:
    Render(QWidget *parent = 0);
    ~Render();


public slots:
    void on_renerTimer();
    virtual void resizeEvent( QResizeEvent * );

    virtual void mouseMoveEvent( QMouseEvent * );

    virtual void mouseReleaseEvent( QMouseEvent * );

    virtual void mousePressEvent( QMouseEvent * );
};

#endif // RENDER_H
