#include "wintest.h"

WinTest::WinTest(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);
    m_render = new Render();
    m_render->show();
}

WinTest::~WinTest()
{

}
