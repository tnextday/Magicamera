#include "winmagic.h"

WinMagic::WinMagic(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);
    m_render.show();
    QStringList sl;
    sl<<QString::fromWCharArray(L"哈哈相机")<<QString::fromWCharArray(L"特效相机");
    ui.cb_engine->addItems(sl);
}

WinMagic::~WinMagic()
{
}

void WinMagic::on_cb_engine_currentIndexChanged( int index )
{
    
}