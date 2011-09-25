#include "winmagic.h"
#include <QFileDialog>

WinMagic::WinMagic(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);
    m_render.show();
    QStringList sl;
    sl<<QString::fromWCharArray(L"哈哈相机")<<QString::fromWCharArray(L"特效相机");
    ui.cb_engine->addItems(sl);
    QDir dir = QDir::current();
    dir.cd("./assets/frames");
    sl = dir.entryList(QStringList("*.png"),QDir::Files | QDir::NoSymLinks);
    sl<<"";
    ui.cmb_cover->addItems(sl);
    dir.cd("../effects");
    sl = dir.entryList(QStringList("*.sp"),QDir::Files | QDir::NoSymLinks);
    sl<<QString::fromWCharArray(L"");
    ui.cmb_effect->addItems(sl);
    dir.cd("../../test");
    m_render.setImage(dir.filePath("test.jpg"));
}

WinMagic::~WinMagic()
{
}

void WinMagic::on_cb_engine_currentIndexChanged( int index )
{
    m_render.setEngine(index+1);
}

void WinMagic::on_clb_save_clicked()
{
    m_render.takePicture();
}

void WinMagic::on_clb_selectImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    m_render.setImage(fileName);
    ui.cb_engine->setCurrentIndex(0);
}

void WinMagic::on_clb_restore_clicked()
{
    m_render.restoreMesh();
}

void WinMagic::on_cmb_cover_currentIndexChanged( const QString & text )
{
    m_render.setCover(QString::fromWCharArray(L"res://frames\\%1").arg(text));
}

void WinMagic::on_cmb_effect_currentIndexChanged( const QString & text )
{
    m_render.setEffect(QString::fromWCharArray(L"res://effects\\%1").arg(text));
}
