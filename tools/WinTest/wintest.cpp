#include "wintest.h"

WinTest::WinTest(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);
    m_render.show();
    QStringList sl;
    sl<<QString::fromWCharArray(L"�������")<<QString::fromWCharArray(L"��Ч���");
    ui.cb_engine->addItems(sl);
}

WinTest::~WinTest()
{
}