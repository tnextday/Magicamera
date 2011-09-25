#include "render.h"
#include <QDir>
#include <QDateTime>
#include <QDebug>

bool WinCallBack::SaveImage( char* buffer, int w, int h, int format )
{
    bool result = false;
    QString imgName = QString("magic_%1.jpg").arg(QDateTime::currentDateTime().toString("yyMMdd_hhmmss_zzz"));
    QImage img((uchar*)buffer, w, h, QImage::Format_ARGB32);
    img = img.rgbSwapped();
    img = img.mirrored();
    if (!m_saveImageDir.exists())
        m_saveImageDir.mkpath("./");
    result = img.save(m_saveImageDir.filePath(imgName), "JPG");
    return result;
}


unsigned char* WinCallBack::readResFile( const char* resname, uint32_t &size )
{
    QFile file(m_resDir.filePath(resname));
    if (!file.open(QIODevice::ReadOnly))
        return NULL;

    char* buffer = NULL;
    size = file.size();
    buffer = new char[size];
    if (file.read(buffer, size) <= 0){
        SafeDeleteArray(buffer);
    }

__end:
    file.close();
    return (unsigned char*)buffer;
}

void WinCallBack::setSavePath(const QString &path )
{
    m_saveImageDir.setPath(path);
}

void WinCallBack::setResDir(const QString &path )
{
    m_resDir.setPath(path);
}


WinCallBack::WinCallBack()
{
    m_resDir.setPath(QString::fromWCharArray(L"assets\\"));
    m_saveImageDir.setPath(QString::fromWCharArray(L"out\\"));
}

Render::Render(QWidget *parent)
    : QWidget(parent, Qt::WFlags(Qt::MSWindowsOwnDC))
{
    setAttribute(Qt::WA_PaintOnScreen);
    //setAttribute(Qt::WA_NoBackground);
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
    qDebug()<<"resizeEvent "<< e->size();
    on_renerTimer();
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

void Render::setImage(const QString &path )
{
    m_MagicMain.setPreviewImage(path.toLocal8Bit());
}

void Render::setEngine( int engine )
{
    if (engine >= ENgineType_Size)
        return;
    m_MagicMain.switchEngine((EngineType)engine);
}

void Render::restoreMesh()
{
    m_MagicMain.restoreMesh();
}

void Render::setCover(const QString &path )
{
    m_MagicMain.setCover(path.toLocal8Bit());
}

void Render::setEffect(const QString &path )
{
    m_MagicMain.setEffect(path.toLocal8Bit());
}

void Render::setSavePath(const QString &path )
{
    m_WinCallBack.setSavePath(path);
}

void Render::setResPath(const QString &path )
{
    m_WinCallBack.setResDir(path);
}

void Render::takePicture()
{
    m_MagicMain.takePicture();
}

// ccording to the Qt docs, if you want to use GDI or Direct3D on Windows with Qt, you need to:
// 
// 1) Override QWidget::paintEngine to return NULL
// 2) Call QWidget::setAttribute(Qt::WA_PaintOnScreen, true)
QPaintEngine* Render::paintEngine() const
{
    return NULL;
}