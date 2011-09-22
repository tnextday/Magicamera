#include "resediter.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ResEditer w;
    w.show();
    return a.exec();
}
