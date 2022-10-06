#include "loatools.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoaTools w;
    w.show();
    return a.exec();
}
