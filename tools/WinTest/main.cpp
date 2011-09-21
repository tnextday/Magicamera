//#include <vld.h>
#include "wintest.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinTest w;
    w.show();
    return a.exec();
}
