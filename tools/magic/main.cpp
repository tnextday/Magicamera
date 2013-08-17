//#include <vld.h>
#include "winmagic.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinMagic w;
    w.show();
    return a.exec();
}
