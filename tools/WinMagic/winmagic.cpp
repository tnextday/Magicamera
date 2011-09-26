#include "winmagic.h"
#include <QFileDialog>

WinMagic::WinMagic(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);
    m_render.show();
    setupTest();
    setupResEditer();
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
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.tga)"));
    if (QFile::exists(fileName)){
        m_render.setImage(fileName);
        //ui.cb_engine->setCurrentIndex(0);
    }
}

void WinMagic::on_clb_restore_clicked()
{
    m_render.restoreMesh();
}

void WinMagic::on_cmb_covers_currentIndexChanged( const QString & text )
{

}

void WinMagic::on_cmb_effect_currentIndexChanged( const QString & text )
{
    if (!text.startsWith("--"))
        m_render.setEffect(QString::fromWCharArray(L"res://effects\\%1").arg(text));
    else
        m_render.setEffect("");
}

void WinMagic::on_tabWidget_currentChanged ( int index )
{
    if (index != 0){
        m_render.setEngine(2);
    }
}


void WinMagic::on_cmb_preset_currentIndexChanged( int index )
{

}

void WinMagic::on_btn_preview_clicked()
{

}

void WinMagic::on_btn_saveRes_clicked()
{

}

void WinMagic::on_btn_selResImg_clicked()
{

}

void WinMagic::setupTest()
{
    QStringList sl;
    sl<<QString::fromWCharArray(L"哈哈相机")<<QString::fromWCharArray(L"特效相机");
    ui.cb_engine->addItems(sl);
    QDir dir = QDir::current();
    dir.cd("./assets/frames");
    sl = dir.entryList(QStringList("*.png"),QDir::Files | QDir::NoSymLinks);
    sl.push_front("--None--");
    ui.cmb_frames->addItems(sl);
    dir.cd("../effects");
    sl = dir.entryList(QStringList("*.sp"),QDir::Files | QDir::NoSymLinks);
    sl.push_front("--None--");
    ui.cmb_effect->addItems(sl);
    dir.cd("../../test");
    m_render.setImage(dir.filePath("test.jpg"));
}

void WinMagic::setupResEditer()
{
    QStringList sl;
    for (int i=0;;i++){
        if (BlendFuncFactors[i].str == "END")
            break;
        ui.cmb_sf->addItem(BlendFuncFactors[i].str);
    }
    for (int i=0;;i++){
        if (BlendFuncFactors[i].str == "GL_SRC_ALPHA_SATURATE")
            break;
        ui.cmb_df->addItem(BlendFuncFactors[i].str);
    }
}

int WinMagic::getGLBlendFuncFromStr( const QString & codeStr )
{
    for (int i=0;;i++){
        if (!BlendFuncFactors[i].code)
            return 0;
        if (BlendFuncFactors[i].str == codeStr)
            return BlendFuncFactors[i].code;
    }
}

void WinMagic::on_cmb_frames_currentIndexChanged( const QString & text )
{
    if (!text.startsWith("--"))
        m_render.setFrame(QString::fromWCharArray(L"res://frames\\%1").arg(text));
    else
        m_render.setFrame("");
}