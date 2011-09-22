#ifndef WINTEST_H
#define WINTEST_H

#include <QtGui/QWidget>
#include "ui_wintest.h"
#include "render.h"

class WinTest : public QWidget
{
    Q_OBJECT
    Render m_render;
public:
    WinTest(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~WinTest();

private:
    Ui::WinTestClass ui;
};

#endif // WINTEST_H
