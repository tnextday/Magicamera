#include "resediter.h"

ResEditer::ResEditer(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);
    m_render.show();
}

ResEditer::~ResEditer()
{

}
