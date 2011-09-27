#include "winmagic.h"
#include <QFileDialog>
#include <QUrl>
#include <QDebug>

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
    if (index < ui.cmb_preset->count() -1){
        ui.cmb_sf->setCurrentIndex(m_sfs.indexOf(BlendFuncFactors[index].sf));
        ui.cmb_df->setCurrentIndex(m_dfs.indexOf(BlendFuncFactors[index].df));
    }
    ui.cmb_sf->setEnabled(ui.cmb_preset->count()-1 == index);
    ui.cmb_df->setEnabled(ui.cmb_preset->count()-1 == index);
}

//预览
void WinMagic::on_btn_preview_clicked()
{
    if (ui.cmb_resType->currentIndex() == 0){
        //保存为蒙版
    }else{
        //保存为相框
    }
}
//保存资源
void WinMagic::on_btn_saveRes_clicked()
{
    if (ui.cmb_resType->currentIndex() == 0){
        //保存为蒙版
    }else{
        //保存为相框
    }
}

void WinMagic::on_btn_selResImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.tga)"));
    if (!QFile::exists(fileName)){
        return;
        //ui.cb_engine->setCurrentIndex(0);
    }
    ui.edt_resImg->setText(fileName);
    ui.edt_name->setText(QFileInfo(fileName).baseName());
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
    for (int i=0;;i++){
        if (BlendFuncs[i].str == "END")
            break;
        m_sfs<<BlendFuncs[i].str;
        
    }
    ui.cmb_sf->addItems(m_sfs);
    for (int i=0;;i++){
        if (BlendFuncs[i].str == "GL_SRC_ALPHA_SATURATE")
            break;
        m_dfs<<BlendFuncs[i].str;
        
    }
    ui.cmb_df->addItems(m_dfs);
    for (int i=0;;i++){
        if (BlendFuncFactors[i].name == "END")
            break;
        ui.cmb_preset->addItem(BlendFuncFactors[i].name);
    }
    ui.cmb_preset->setCurrentIndex(0);
    on_cmb_preset_currentIndexChanged(0);
    ui.cmb_preset->addItem("User-Define");
    ui.cmb_resType->addItem("Edit Cover");
    ui.cmb_resType->addItem("Edit Frame");
}

int WinMagic::getGLBlendFuncFromStr( const QString & codeStr )
{
    for (int i=0;;i++){
        if (BlendFuncs[i].str == "END")
            return 0;
        if (BlendFuncs[i].str == codeStr)
            return BlendFuncs[i].code;
    }
}

void WinMagic::on_cmb_frames_currentIndexChanged( const QString & text )
{
    if (!text.startsWith("--"))
        m_render.setFrame(QString::fromWCharArray(L"res://frames\\%1").arg(text));
    else
        m_render.setFrame("");
}

void WinMagic::on_cmb_resType_currentIndexChanged( int index )
{
    if (index == 0){
        //编辑蒙版，取消相框
        m_render.setFrame("");
    }else{
        m_render.setCover("");
    }
}

void WinMagic::dropEvent( QDropEvent * event)
{
    QFileInfo fi;
    if (event->mimeData()->hasUrls()) {
        foreach (QUrl url, event->mimeData()->urls()) {
            //qDebug()<<url.toLocalFile();
            fi.setFile(url.toLocalFile());
            if (QString("tga jpg jpeg bmp png").contains(fi.suffix().toLower())){
                ui.edt_resImg->setText(fi.filePath());
                break;
            }
        }
    }
}

void WinMagic::dragEnterEvent( QDragEnterEvent * event)
{
    
    if (event->mimeData()->hasUrls() /*&& ui.edt_resImg->geometry().contains(event->pos())*/)
        event->acceptProposedAction();
}