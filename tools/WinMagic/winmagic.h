#ifndef WINTEST_H
#define WINTEST_H

#include <QtGui/QWidget>
#include "ui_winmagic.h"
#include "render.h"

class WinMagic : public QWidget
{
    Q_OBJECT
    Render m_render;
public:
    WinMagic(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~WinMagic();

private:
    Ui::WinTestClass ui;

public slots:
    void on_cb_engine_currentIndexChanged(int index);
};

#endif // WINTEST_H
