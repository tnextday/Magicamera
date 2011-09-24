#include "render.h"
#include <QDir>
#include <QDateTime>

bool WinCallBack::SaveImage( char* buffer, int w, int h, int format )
{
    bool result = false;
    QString imgName = QString("magic_%1.jpg").arg(QDateTime::currentDateTime().toString("YYMMDD_hhmmss_sss"));
    QImage img(w, h, QImage::Format_RGB888);
    if (img.loadFromData((const uchar*)buffer, w*h*4)){
        img.rgbSwapped();
        if (!m_saveImageDir.exists())
            m_saveImageDir.mkpath("");
        result = img.save(m_saveImageDir.filePath(imgName), "JPG");
    }
    return result;
}


unsigned char* WinCallBack::readResFile( const char* resname, uint32_t &size )
{
    return (unsigned char*)readResFile(m_resDir.filePath(resname), (uint32_t)size);
}

void WinCallBack::setSavePath( QString &path )
{
    m_saveImageDir.setPath(path);
}

void WinCallBack::setResDir( QString &path )
{
    m_resDir.setPath(path);
}

char* WinCallBack::readResFile( QString &filename, uint32_t &size, char* preBuffer /*= NULL*/ )
{
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

WinCallBack::WinCallBack()
{
    m_resDir.setPath(QString::fromWCharArray(L"assets\\"));
    m_saveImageDir.setPath(QString::fromWCharArray(L"out\\"));
}

Render::Render(QWidget *parent)
    : QWidget(parent)
{
    this->resize(640, 480);
    if (!eglCreateSurface(this->winId(), m_eglSurface, m_eglDisplay))
        return;

    m_MagicMain.setupGraphics();
    m_MagicMain.setIOCallBack(&m_WinCallBack);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_renerTimer()));
    m_timer.setSingleShot(false);
    m_timer.start(1000/60);
    m_fpsTime = 0;
    m_lastTime = 0;
    m_fpsFrames = 0;
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

void Render::setImage( QString &path )
{
    m_MagicMain.setPreviewImage(path.toLocal8Bit());
}

void Render::setEngine( int engine )
{

}

void Render::restoreMesh()
{

}

void Render::setCover( QString &path )
{

}

void Render::setEffect( QString &path )
{

}

void Render::setSavePath( QString &path )
{

}

void Render::setResPath( QString &path )
{

}