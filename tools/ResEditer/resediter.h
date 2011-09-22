#ifndef RESEDITER_H
#define RESEDITER_H

#include <QtGui/QWidget>
#include "ui_resediter.h"
#include "render.h"

class ResEditer : public QWidget
{
    Q_OBJECT
    Render m_render;
public:
    ResEditer(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~ResEditer();

private:
    Ui::ResEditerClass ui;
};

#endif // RESEDITER_H
