#ifndef RENDER_H
#define RENDER_H
#include <QWidget>
#include <EGL/egl.h>
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>
#include "magic/magicmain.h"
#include <QTimer>
#include <QResizeEvent>
#include <QDir>
#include "eglhelper.h"


class WinCallBack: public FileUtils{
private:
    QDir     m_saveImageDir;
    QDir     m_resDir;
public:
    WinCallBack();
    virtual bool SaveImage( char* buffer, int w, int h, int format );
    virtual unsigned char* readResFile( const char* resname, uint32_t& size );

    void setSavePath(QString &path);
    void setResDir(QString &path);
    static char* readResFile(QString &filename, uint32_t &size, char* preBuffer = NULL);
};


class Render : public QWidget
{
    Q_OBJECT
private:
    float       m_lastTime;
    float       m_fpsTime;
    int         m_fpsFrames;
public:
    MagicMain       m_MagicMain;
    WinCallBack     m_WinCallBack;
    EGLSurface      m_eglSurface;
    EGLDisplay      m_eglDisplay;

    QTimer          m_timer;
public:
    Render(QWidget *parent = 0);
    ~Render();

public slots:
    void on_renerTimer();
    virtual void resizeEvent( QResizeEvent * e);
    virtual void mouseMoveEvent( QMouseEvent * e);
    virtual void mouseReleaseEvent( QMouseEvent * e);
    virtual void mousePressEvent( QMouseEvent * e);

    void setImage(QString &path);
    void setEngine(int engine);
    void restoreMesh();
    void setCover(QString &path);
    void setEffect(QString &path);
    void setSavePath(QString &path);
    void setResPath(QString &path);


};

#endif // RENDER_H
