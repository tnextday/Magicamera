#ifndef RENDER_H
#define RENDER_H

#include <QWidget>
#include <QTimer>
#include <EGL/egl.h>
#include <GLES2/GL2.h>
#include <GLES2/gl2ext.h>
#include "magic/magicmain.h"

class Render : public QWidget
{
    Q_OBJECT
public:
    QTimer          m_timer;
    EGLSurface      m_eglSurface;
    EGLDisplay      m_eglDisplay;

public:
    Render(QWidget *parent = 0);
    ~Render();

private:
   
public slots:
    void on_renerTimer();
    virtual void resizeEvent( QResizeEvent * e);
};

#endif // RENDER_H
